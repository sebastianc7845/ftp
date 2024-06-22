#include "server.hpp"

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
        } else if (ready_poll > 0) {
            Command cmd;
            ssize_t ret;

            // TODO: handle client input
            LOGF("receiving from user @ idx %d", GET_USER_IDX(ready_poll));
            // receive client command
            ret = srv.ReceiveCommand(GET_USER_IDX(ready_poll), &cmd);

            if (ret > 0) {
                const char msg[] = "Hello"; 
                Response rsp('+', msg, strlen(msg));

                LOGF("%s", "received command: ");
                cmd.printCommand();

                LOGF("%s", "sending response...");
                srv.SendResponse(GET_USER_IDX(ready_poll), &rsp);
            }
        }
    }
    // end_loop

    return 0;
}