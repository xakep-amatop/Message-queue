#include <iostream>
#include <fstream>

#include <cstring>

#include <errno.h>

#include <fcntl.h>
#include <mqueue.h>

#include "message_info.pb.h"
#include "common.h"

using namespace std;

char const * const type_log_msg[] = {
  "\033[0;36mDEBUG\033[0m", // letter have cyan     color
  "\033[0;32mINFO\033[0m",  // letter have green    color
  "\033[0;35mWARN\033[0m",  // letter have magneta  color
  "\033[0;31mERROR\033[0m", // letter have red      color
};

int main(int argc, char** argv) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  mqd_t mq;                   // message queue descriptor
  struct mq_attr attr = {0};  // message queue atributes

  /* initialize the queue attributes */
  attr.mq_maxmsg    = MAX_MQ_MSG_NUM;
  attr.mq_msgsize   = MAX_MQ_MSG_SIZE;

  /* create the message queue,  (user c can read/write, others only read)*/
  mq = mq_open(MQ_PATH_NAME, O_CREAT | O_RDONLY, 0644, &attr);

  if (mq == (mqd_t) -1){
    cerr << "mq_open: " << strerror(errno) << endl;
    return EXIT_FAILURE;
  }

  char buffer[MAX_MQ_MSG_SIZE + 1];

  msg_info::msg log_msg;  // object that will be parse our receive buffer

  while (true){
    /* receive the message */
    ssize_t bytes_read = mq_receive(mq, buffer, MAX_MQ_MSG_SIZE, NULL);

    if (bytes_read == -1){
      cerr << "mq_receive: "<< strerror(errno) << endl;
      continue;
    }

    string str = string(buffer);

    log_msg.ParseFromString(str);
    if (log_msg.has_from_id()) {
      cout << "\nID: "<<log_msg.from_id();
    }
    if (log_msg.has_name()) {
      cout << "\nNAME: "<<log_msg.name();
    }

    cout << "\n" << type_log_msg[log_msg.type() - 1] << ": ";

    if (log_msg.has_data()) {
      cout << log_msg.data();
    }

    cout << endl;

  }

  // /* cleanup */
  // if (mq_close(mq) == -1){
  //   cerr << "mq_close: "<< strerror(errno) << endl;
  // }

  // if (mq_unlink(MQ_PATH_NAME) == -1){
  //   cerr << "mq_unlink: "<< strerror(errno) << endl;
  // }

  return EXIT_SUCCESS;
}