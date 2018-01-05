#include <iostream>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sstream>

#include <mqueue.h>

#include "message_info.pb.h"
#include "common.h"

void	error_func(std::string str)
{
	std::cout << str << ": " << strerror(errno) << std::endl;
	exit(0);
}

int	main(int argc, char **argv)
{
	mqd_t			mq;
	msg_info::msg		msg_data;
	int			msg_type;
	std::string		buf;
	struct			mq_attr	attr = {0};
	std::string		messages[8] = {
		"This message is supposed to tell that client is debugging something.\n",
		"Client	is almost done debugging or whatever.\n",
		"Just some INFO information.\n",
		"Just a typical message nonsense.\n",
		"It's a warning! Be warned!.\n",
		"CAREFUL BE NOW YOU!\n",
		"Error is inevitable...\n",
		"Error time!\n"
	};	//	"random" messages array

	attr.mq_maxmsg = MAX_MQ_MSG_NUM;
	attr.mq_msgsize = MAX_MQ_MSG_SIZE;

		//	Opening message queue
	mq = mq_open(MQ_PATH_NAME, O_WRONLY | O_CREAT, 0644, &attr);
	if (mq == (mqd_t) - 1)
		error_func("Couldn't open message queue");

	srand(time(0));
	std::cout << "Sending messages...\n";

	while (1)
	{
		msg_data.Clear();
		msg_type = rand() % 4 + 1;
		msg_data.set_type((msg_info::msg__TYPE)msg_type);
		msg_data.set_name(argv[0]);
		msg_data.set_from_id(std::to_string(getpid()));
		msg_data.set_data(messages[msg_type * 2 - 1 - rand() % 2]);
		if (!msg_data.SerializeToString(&buf))
      		error_func("Failed to write message.");
		if (mq_send(mq, buf.c_str(), buf.length(), 0))
			error_func("Couln't send a message");
		sleep(1);
	}
	mq_close(mq);
	return (0);
}
