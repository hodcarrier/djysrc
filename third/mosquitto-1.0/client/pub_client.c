/*
Copyright (c) 2009-2012 Roger Light <roger@atchoo.org>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. Neither the name of mosquitto nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/


#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#include <unistd.h>
#else
#include <process.h>
#include <winsock2.h>
#define snprintf sprintf_s
#endif

#include <mosquitto.h>

#define MSGMODE_NONE 0
#define MSGMODE_CMD 1
#define MSGMODE_STDIN_LINE 2
#define MSGMODE_STDIN_FILE 3
#define MSGMODE_FILE 4
#define MSGMODE_NULL 5

#define STATUS_CONNECTING 0
#define STATUS_CONNACK_RECVD 1

/* Global variables for use in callbacks. See sub_client.c for an example of
 * using a struct to hold variables for use in callbacks. */
static char *topic = NULL;
static char *message = NULL;
static long msglen = 0;
static int qos = 0;
static int retain = 0;
static int mode = MSGMODE_NONE;
static int status = STATUS_CONNECTING;
static int mid_sent = 0;
static bool connected = true;
static char *username = NULL;
static char *password = NULL;
static bool disconnect_sent = false;
static bool quiet = false;


static void ResetConfig(void)
{
	topic = NULL;
	message = NULL;
	msglen = 0;
	qos = 0;
	retain = 0;
	mode = MSGMODE_NONE;
	status = STATUS_CONNECTING;
	mid_sent = 0;
	connected = true;
	username = NULL;
	password = NULL;
	disconnect_sent = false;
	quiet = false;
	return;
}

static void my_connect_callback(struct mosquitto *mosq, void *obj, int result)
{
	int rc = MOSQ_ERR_SUCCESS;

	if(!result){
		switch(mode){
			case MSGMODE_CMD:
			case MSGMODE_FILE:
			case MSGMODE_STDIN_FILE:
				rc = mosquitto_publish(mosq, &mid_sent, topic, msglen, message, qos, retain);
				break;
			case MSGMODE_NULL:
				rc = mosquitto_publish(mosq, &mid_sent, topic, 0, NULL, qos, retain);
				break;
			case MSGMODE_STDIN_LINE:
				status = STATUS_CONNACK_RECVD;
				break;
		}
		if(rc){
			if(!quiet){
				switch(rc){
					case MOSQ_ERR_INVAL:
						fprintf(stderr, "Error: Invalid input. Does your topic contain '+' or '#'?\n\r");
						break;
					case MOSQ_ERR_NOMEM:
						fprintf(stderr, "Error: Out of memory when trying to publish message.\n\r");
						break;
					case MOSQ_ERR_NO_CONN:
						fprintf(stderr, "Error: Client not connected when trying to publish.\n\r");
						break;
					case MOSQ_ERR_PROTOCOL:
						fprintf(stderr, "Error: Protocol error when communicating with broker.\n\r");
						break;
					case MOSQ_ERR_PAYLOAD_SIZE:
						fprintf(stderr, "Error: Message payload is too large.\n\r");
						break;
				}
			}
			mosquitto_disconnect(mosq);
		}
	}else{
		if(result && !quiet){
			fprintf(stderr, "%s\n\r", mosquitto_connack_string(result));
		}
	}
}

static void my_disconnect_callback(struct mosquitto *mosq, void *obj, int rc)
{
	connected = false;
}

static void my_publish_callback(struct mosquitto *mosq, void *obj, int mid)
{
	if(mode != MSGMODE_STDIN_LINE && disconnect_sent == false){
		mosquitto_disconnect(mosq);
		disconnect_sent = true;
	}
}

static void my_log_callback(struct mosquitto *mosq, void *obj, int level, const char *str)
{
	printf("%s\n\r", str);
}

static int load_stdin(void)
{
	long pos = 0, rlen;
	char buf[1024];

	mode = MSGMODE_STDIN_FILE;

	while(!feof(stdin)){
		rlen = fread(buf, 1, 1024, stdin);
		message = realloc(message, pos+rlen);
		if(!message){
			if(!quiet) fprintf(stderr, "Error: Out of memory.\n\r");
			return 1;
		}
		memcpy(&(message[pos]), buf, rlen);
		pos += rlen;
	}
	msglen = pos;

	if(!msglen){
		if(!quiet) fprintf(stderr, "Error: Zero length input.\n\r");
		return 1;
	}

	return 0;
}

static int load_file(const char *filename)
{
	long pos, rlen;
	FILE *fptr = NULL;

	fptr = fopen(filename, "rb");
	if(!fptr){
		if(!quiet) fprintf(stderr, "Error: Unable to open file \"%s\".\n\r", filename);
		return 1;
	}
	mode = MSGMODE_FILE;
	fseek(fptr, 0, SEEK_END);
	msglen = ftell(fptr);
	if(msglen > 268435455){
		fclose(fptr);
		if(!quiet) fprintf(stderr, "Error: File \"%s\" is too large (>268,435,455 bytes).\n\r", filename);
		return 1;
	}
	if(msglen == 0){
		fclose(fptr);
		if(!quiet) fprintf(stderr, "Error: File \"%s\" is empty.\n\r", filename);
		return 1;
	}
	fseek(fptr, 0, SEEK_SET);
	message = malloc(msglen);
	if(!message){
		fclose(fptr);
		if(!quiet) fprintf(stderr, "Error: Out of memory.\n\r");
		return 1;
	}
	pos = 0;
	while(pos < msglen){
		rlen = fread(&(message[pos]), sizeof(char), msglen-pos, fptr);
		pos += rlen;
	}
	fclose(fptr);
	return 0;
}

static void print_usage(void)
{
	printf("mosquitto_pub is a simple mqtt client that will publish a message on a single topic and exit.\n\r\n\r");
	printf("Usage: mosquitto_pub [-h host] [-p port] [-q qos] [-r] {-f file | -l | -n | -m message} -t topic\n\r");
	printf("                     [-i id] [-I id_prefix]\n\r");
	printf("                     [-d] [--quiet]\n\r");
	printf("                     [-u username [-P password]]\n\r");
	printf("                     [--will-topic [--will-payload payload] [--will-qos qos] [--will-retain]]\n\r");
	printf("                     [{--cafile file | --capath dir} [--cert file] [--key file]]\n\r");
	printf("                     [--psk hex-key --psk-identity identity]\n\r");
	printf("       mosquitto_pub --help\n\r\n\r");
	printf(" -d : enable debug messages.\n\r");
	printf(" -f : send the contents of a file as the message.\n\r");
	printf(" -h : mqtt host to connect to. Defaults to localhost.\n\r");
	printf(" -i : id to use for this client. Defaults to mosquitto_pub_ appended with the process id.\n\r");
	printf(" -I : define the client id as id_prefix appended with the process id. Useful for when the\n\r");
	printf("      broker is using the clientid_prefixes option.\n\r");
	printf(" -l : read messages from stdin, sending a separate message for each line.\n\r");
	printf(" -m : message payload to send.\n\r");
	printf(" -n : send a null (zero length) message.\n\r");
	printf(" -p : network port to connect to. Defaults to 1883.\n\r");
	printf(" -q : quality of service level to use for all messages. Defaults to 0.\n\r");
	printf(" -r : message should be retained.\n\r");
	printf(" -s : read message from stdin, sending the entire input as a message.\n\r");
	printf(" -t : mqtt topic to publish to.\n\r");
	printf(" -u : provide a username (requires MQTT 3.1 broker)\n\r");
	printf(" -P : provide a password (requires MQTT 3.1 broker)\n\r");
	printf(" --help : display this message.\n\r");
	printf(" --quiet : don't print error messages.\n\r");
	printf(" --will-payload : payload for the client Will, which is sent by the broker in case of\n\r");
	printf("                  unexpected disconnection. If not given and will-topic is set, a zero\n\r");
	printf("                  length message will be sent.\n\r");
	printf(" --will-qos : QoS level for the client Will.\n\r");
	printf(" --will-retain : if given, make the client Will retained.\n\r");
	printf(" --will-topic : the topic on which to publish the client Will.\n\r");
	printf(" --cafile : path to a file containing trusted CA certificates to enable encrypted\n\r");
	printf("            communication.\n\r");
	printf(" --capath : path to a directory containing trusted CA certificates to enable encrypted\n\r");
	printf("            communication.\n\r");
	printf(" --cert : client certificate for authentication, if required by server.\n\r");
	printf(" --key : client private key for authentication, if required by server.\n\r");
	printf(" --psk : pre-shared-key in hexadecimal (no leading 0x) to enable TLS-PSK mode.\n\r");
	printf(" --psk-identity : client identity string for TLS-PSK mode.\n\r");
	printf("\n\rSee http://mosquitto.org/ for more information.\n\r\n\r");
}

int Pubmain(int argc, char *argv[])
{
	char *id = NULL;
	char *id_prefix = NULL;
	int i;
	char *host = "localhost";
	int port = 1883;
	int keepalive = 60;
#ifndef WIN32
	int opt;
#endif
	char buf[1024];
	bool debug = false;
	struct mosquitto *mosq = NULL;
	int rc;
	int rc2;
	char hostname[256];
	char err[1024];
	int len;

	char *will_payload = NULL;
	long will_payloadlen = 0;
	int will_qos = 0;
	bool will_retain = false;
	char *will_topic = NULL;

	char *cafile = NULL;
	char *capath = NULL;
	char *certfile = NULL;
	char *keyfile = NULL;

	char *psk = NULL;
	char *psk_identity = NULL;

	for(i=1; i<argc; i++){
		if(!strcmp(argv[i], "-p") || !strcmp(argv[i], "--port")){
			if(i==argc-1){
				fprintf(stderr, "Error: -p argument given but no port specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				port = atoi(argv[i+1]);
				if(port<1 || port>65535){
					fprintf(stderr, "Error: Invalid port given: %d\n\r", port);
					print_usage();
					return 1;
				}
			}
			i++;
		}else if(!strcmp(argv[i], "--cafile")){
			if(i==argc-1){
				fprintf(stderr, "Error: --cafile argument given but no file specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				cafile = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "--capath")){
			if(i==argc-1){
				fprintf(stderr, "Error: --capath argument given but no directory specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				capath = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "--cert")){
			if(i==argc-1){
				fprintf(stderr, "Error: --cert argument given but no file specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				certfile = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug")){
			debug = true;
		}else if(!strcmp(argv[i], "-f") || !strcmp(argv[i], "--file")){
			if(mode != MSGMODE_NONE){
				fprintf(stderr, "Error: Only one type of message can be sent at once.\n\r\n\r");
				print_usage();
				return 1;
			}else if(i==argc-1){
				fprintf(stderr, "Error: -f argument given but no file specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				if(load_file(argv[i+1])) return 1;
			}
			i++;
		}else if(!strcmp(argv[i], "--help")){
			print_usage();
			return 0;
		}else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--host")){
			if(i==argc-1){
				fprintf(stderr, "Error: -h argument given but no host specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				host = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "-i") || !strcmp(argv[i], "--id")){
			if(id_prefix){
				fprintf(stderr, "Error: -i and -I argument cannot be used together.\n\r\n\r");
				print_usage();
				return 1;
			}
			if(i==argc-1){
				fprintf(stderr, "Error: -i argument given but no id specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				id = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "-I") || !strcmp(argv[i], "--id-prefix")){
			if(id){
				fprintf(stderr, "Error: -i and -I argument cannot be used together.\n\r\n\r");
				print_usage();
				return 1;
			}
			if(i==argc-1){
				fprintf(stderr, "Error: -I argument given but no id prefix specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				id_prefix = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "--key")){
			if(i==argc-1){
				fprintf(stderr, "Error: --key argument given but no file specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				keyfile = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "-l") || !strcmp(argv[i], "--stdin-line")){
			if(mode != MSGMODE_NONE){
				fprintf(stderr, "Error: Only one type of message can be sent at once.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				mode = MSGMODE_STDIN_LINE;
#ifndef WIN32
				opt = fcntl(fileno(stdin), F_GETFL, 0);
				if(opt == -1 || fcntl(fileno(stdin), F_SETFL, opt | O_NONBLOCK) == -1){
					fprintf(stderr, "Error: Unable to set stdin to non-blocking.\n\r");
					return 1;
				}
#endif
			}
		}else if(!strcmp(argv[i], "-m") || !strcmp(argv[i], "--message")){
			if(mode != MSGMODE_NONE){
				fprintf(stderr, "Error: Only one type of message can be sent at once.\n\r\n\r");
				print_usage();
				return 1;
			}else if(i==argc-1){
				fprintf(stderr, "Error: -m argument given but no message specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				message = argv[i+1];
				msglen = strlen(message);
				mode = MSGMODE_CMD;
			}
			i++;
		}else if(!strcmp(argv[i], "-n") || !strcmp(argv[i], "--null-message")){
			if(mode != MSGMODE_NONE){
				fprintf(stderr, "Error: Only one type of message can be sent at once.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				mode = MSGMODE_NULL;
			}
		}else if(!strcmp(argv[i], "--psk")){
			if(i==argc-1){
				fprintf(stderr, "Error: --psk argument given but no key specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				psk = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "--psk-identity")){
			if(i==argc-1){
				fprintf(stderr, "Error: --psk-identity argument given but no identity specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				psk_identity = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "-q") || !strcmp(argv[i], "--qos")){
			if(i==argc-1){
				fprintf(stderr, "Error: -q argument given but no QoS specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				qos = atoi(argv[i+1]);
				if(qos<0 || qos>2){
					fprintf(stderr, "Error: Invalid QoS given: %d\n\r", qos);
					print_usage();
					return 1;
				}
			}
			i++;
		}else if(!strcmp(argv[i], "--quiet")){
			quiet = true;
		}else if(!strcmp(argv[i], "-r") || !strcmp(argv[i], "--retain")){
			retain = 1;
		}else if(!strcmp(argv[i], "-s") || !strcmp(argv[i], "--stdin-file")){
			if(mode != MSGMODE_NONE){
				fprintf(stderr, "Error: Only one type of message can be sent at once.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				if(load_stdin()) return 1;
			}
		}else if(!strcmp(argv[i], "-t") || !strcmp(argv[i], "--topic")){
			if(i==argc-1){
				fprintf(stderr, "Error: -t argument given but no topic specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				topic = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "-u") || !strcmp(argv[i], "--username")){
			if(i==argc-1){
				fprintf(stderr, "Error: -u argument given but no username specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				username = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "-P") || !strcmp(argv[i], "--pw")){
			if(i==argc-1){
				fprintf(stderr, "Error: -P argument given but no password specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				password = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "--will-payload")){
			if(i==argc-1){
				fprintf(stderr, "Error: --will-payload argument given but no will payload specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				will_payload = argv[i+1];
				will_payloadlen = strlen(will_payload);
			}
			i++;
		}else if(!strcmp(argv[i], "--will-qos")){
			if(i==argc-1){
				fprintf(stderr, "Error: --will-qos argument given but no will QoS specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				will_qos = atoi(argv[i+1]);
				if(will_qos < 0 || will_qos > 2){
					fprintf(stderr, "Error: Invalid will QoS %d.\n\r\n\r", will_qos);
					return 1;
				}
			}
			i++;
		}else if(!strcmp(argv[i], "--will-retain")){
			will_retain = true;
		}else if(!strcmp(argv[i], "--will-topic")){
			if(i==argc-1){
				fprintf(stderr, "Error: --will-topic argument given but no will topic specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				will_topic = argv[i+1];
			}
			i++;
		}else{
			fprintf(stderr, "Error: Unknown option '%s'.\n\r",argv[i]);
			print_usage();
			return 1;
		}
	}

	if(!topic || mode == MSGMODE_NONE){
		fprintf(stderr, "Error: Both topic and message must be supplied.\n\r");
		print_usage();
		return 1;
	}

	if(will_payload && !will_topic){
		fprintf(stderr, "Error: Will payload given, but no will topic given.\n\r");
		print_usage();
		return 1;
	}
	if(will_retain && !will_topic){
		fprintf(stderr, "Error: Will retain given, but no will topic given.\n\r");
		print_usage();
		return 1;
	}
	if(password && !username){
		if(!quiet) fprintf(stderr, "Warning: Not using password since username not set.\n\r");
	}
	if((certfile && !keyfile) || (keyfile && !certfile)){
		fprintf(stderr, "Error: Both certfile and keyfile must be provided if one of them is.\n\r");
		print_usage();
		return 1;
	}
	if((cafile || capath) && psk){
		if(!quiet) fprintf(stderr, "Error: Only one of --psk or --cafile/--capath may be used at once.\n\r");
		return 1;
	}
	if(psk && !psk_identity){
		if(!quiet) fprintf(stderr, "Error: --psk-identity required if --psk used.\n\r");
		return 1;
	}

	mosquitto_lib_init();

	if(id_prefix){
		id = malloc(strlen(id_prefix)+10);
		if(!id){
			if(!quiet) fprintf(stderr, "Error: Out of memory.\n\r");
			mosquitto_lib_cleanup();
			return 1;
		}
		snprintf(id, strlen(id_prefix)+10, "%s%d", id_prefix, getpid());
	}else if(!id){
		hostname[0] = '\0';
		gethostname(hostname, 256);
		hostname[255] = '\0';
		len = strlen("mosqpub/-") + 6 + strlen(hostname);
		id = malloc(len);
		if(!id){
			if(!quiet) fprintf(stderr, "Error: Out of memory.\n\r");
			mosquitto_lib_cleanup();
			return 1;
		}
		snprintf(id, len, "mosqpub/%d-%s", getpid(), hostname);
		id[MOSQ_MQTT_ID_MAX_LENGTH] = '\0';
	}

	mosq = mosquitto_new(id, true, NULL);
	if(!mosq){
		switch(errno){
			case ENOMEM:
				if(!quiet) fprintf(stderr, "Error: Out of memory.\n\r");
				break;
			case EINVAL:
				if(!quiet) fprintf(stderr, "Error: Invalid id.\n\r");
				break;
		}
		mosquitto_lib_cleanup();
		return 1;
	}
	if(debug){
		mosquitto_log_callback_set(mosq, my_log_callback);
	}
	if(will_topic && mosquitto_will_set(mosq, will_topic, will_payloadlen, will_payload, will_qos, will_retain)){
		if(!quiet) fprintf(stderr, "Error: Problem setting will.\n\r");
		mosquitto_lib_cleanup();
		return 1;
	}
	if(username && mosquitto_username_pw_set(mosq, username, password)){
		if(!quiet) fprintf(stderr, "Error: Problem setting username and password.\n\r");
		mosquitto_lib_cleanup();
		return 1;
	}
	if((cafile || capath) && mosquitto_tls_set(mosq, cafile, capath, certfile, keyfile, NULL)){
		if(!quiet) fprintf(stderr, "Error: Problem setting TLS options.\n\r");
		mosquitto_lib_cleanup();
		return 1;
	}
	if(psk && mosquitto_tls_psk_set(mosq, psk, psk_identity, NULL)){
		if(!quiet) fprintf(stderr, "Error: Problem setting TLS-PSK options.\n\r");
		mosquitto_lib_cleanup();
		return 1;
	}
	mosquitto_connect_callback_set(mosq, my_connect_callback);
	mosquitto_disconnect_callback_set(mosq, my_disconnect_callback);
	mosquitto_publish_callback_set(mosq, my_publish_callback);

	rc = mosquitto_connect(mosq, host, port, keepalive);
	if(rc){
		if(!quiet){
			if(rc == MOSQ_ERR_ERRNO){
#ifndef WIN32
				strerror_r(errno, err, 1024);
#else
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errno, 0, (LPTSTR)&err, 1024, NULL);
#endif
				fprintf(stderr, "Error: %s\n\r", err);
			}else{
				fprintf(stderr, "Unable to connect (%d).\n\r", rc);
			}
		}
		mosquitto_lib_cleanup();
		return rc;
	}

	do{
		if(mode == MSGMODE_STDIN_LINE && status == STATUS_CONNACK_RECVD){
			if(fgets(buf, 1024, stdin)){
				buf[strlen(buf)-1] = '\0';
				rc2 = mosquitto_publish(mosq, &mid_sent, topic, strlen(buf), buf, qos, retain);
				if(rc2){
					if(!quiet) fprintf(stderr, "Error: Publish returned %d, disconnecting.\n\r", rc2);
					mosquitto_disconnect(mosq);
				}
			}else if(feof(stdin) && disconnect_sent == false){
				mosquitto_disconnect(mosq);
				disconnect_sent = true;
			}
		}
		rc = mosquitto_loop(mosq, -1, 1);
	}while(rc == MOSQ_ERR_SUCCESS && connected);

	if(message && mode == MSGMODE_FILE){
		free(message);
	}
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	//clear the config
	ResetConfig();

	if(rc){
		if(rc == MOSQ_ERR_ERRNO){
			fprintf(stderr, "Error: %s\n\r", strerror(errno));
		}else{
			fprintf(stderr, "Error: %s\n\r", mosquitto_strerror(rc));
		}
	}
	return rc;
}



