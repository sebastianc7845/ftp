#include "server.hpp"
#include "reply.hpp"

int main(int argc, char* argv[]) {
    int ready_poll;
    UNUSED(argc);
    UNUSED(argv);

    Server srv;

    // while true
    while (true) {
        ready_poll = srv.CheckPolls();

        if (ready_poll != -2)
            LOGF("ready poll idx: %d", ready_poll);

        if (ready_poll == STDIN_POLL) {
            // TODO: handle console input
        } else if (ready_poll == LISTEN_POLL) {
            LOGF("%s", "user attempting to connect");
            srv.AcceptUser();
            // send success code to user
        } else if (ready_poll > 0) {
            Command cmd;
            ssize_t ret;

            // TODO: handle client input
            LOGF("receiving from user @ idx %d", GET_USER_IDX(ready_poll));
            // receive client command
            ret = srv.ReceiveCommand(GET_USER_IDX(ready_poll), &cmd);

            if (ret > 0) {
                // if client command was successfully received and parsed...
                // process request and send appropriate response

                // switch (command.type)
                switch (cmd.getCommandType()) {
                case USER:
                    {
                    // first command sent by user
                    // for now, accept any username
                    // and just send a success code
                    ssize_t buflen = 0;
                    uint8_t *buf = nullptr;

                    Reply success("200", "Login Successful!", 18);
                    buf = success.serialize(&buflen);

                    srv.SendBytes(GET_USER_IDX(ready_poll), buf, buflen);
                    break;
                    }
                case QUIT:
                    // if transfer in progress
                        // let transfer finish

                    // send success code
                    // close connection to user
                    // delete/close/flush any data structures associated with this user
                    break;
                case PORT:
                    // update user's data connection port
                    // send success code
                    break;
                case TYPE:
                    // set user's data connection type
                    // only accepted type is NVT-ASCII for now
                    break;
                case MODE:
                    // set user's data connection mode
                    // only STREAM for now
                    break;
                case STRU:
                    // set user's data connection structure
                    // only FILE for now
                    break;
                case RETR:
                    // MOST INVOLVED COMMAND
                    // open data connection and facilitate transfer using user's selected
                    // data connection options
                case STOR:
                    // ANOTHER INVOLVED COMMAND
                    // open data connection and facilitate transfer
                    // according to user's data connection options
                    break;
                case NOOP:
                    // no operation lol
                    break;
                }

                // const char msg[] = "Hello"; 
                // Response rsp('+', msg, strlen(msg));

                // LOGF("%s", "received command: ");
                // cmd.printCommand();

                // LOGF("%s", "sending response...");
                // srv.SendResponse(GET_USER_IDX(ready_poll), &rsp);
            }
        }
    }
    // end_loop

    return 0;
}

// TODO: what should this return?
// where should this function go (in main, or server member func)
int processRequest (Server &srv, int user_idx) {
    UNUSED(srv);
    UNUSED(user_idx);
    // MINIMUM IMPLEMENTATION
    // receive command from user

   return 0;
}