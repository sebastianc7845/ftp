#include <iostream>
#include <string>
#include <pwd.h>
#include "client.hpp"
#include "command.hpp"
#include "response.hpp"

void TestProtocol(Client *);
void GetInput(Client &, std::string &);
void Prompt(const char[]);

int main(int argc, char* argv[]) {
    UNUSED(argc);
    UNUSED(argv);

    Client cli;
    std::string input_buf;

    struct passwd *pw;
    uid_t uid;

    uid = geteuid();
    pw = getpwuid(uid);

    if (pw == NULL) {
        LOGERRNO;
        LOGF("%s", "Could not get username, setting default username...");
        cli.SetUsername("user");
    } else {
        cli.SetUsername(pw->pw_name);
    }

    // initiate connection to server on default protcol interpreter port

    cli.Connect();

    // start listening on negotiated data connection port
    // otherwise, listen on default data connection port

    // send transfer command thru protocol interpreter ports

    // if server successfully connects to data port
        // start transferring specified file

    TestProtocol(&cli);

    return 0;
}

void Prompt(const char prompt[]) {
    std::cout << "<" << prompt << ">\n";
}

void GetInput(Client &cli, std::string &inbuf) {
    if (cli.GetUsername().empty() == false)
        std::cout << cli.GetUsername() << ": ";

    std::getline(std::cin, inbuf);

    if (inbuf.compare("Q") == 0 || inbuf.compare("q") == 0) {
        LOGF("%s", "user typed q, quiting program...");
        exit(EXIT_SUCCESS);
    }
}


void TestProtocol(Client *cli) {
    Reply rpy;

    LOGF("%s", "attempting to connect to server");
    if (cli->Connect() < 0)
        LOGERRNO;

    LOGF("%s", "Sending a message to the server...");

    Command cmd(USER, cli->GetUsername().c_str(), cli->GetUsername().size());

    if (cli->SendCommand(&cmd) < 0)
        LOGERRNO;

    // const char msg[] = "Hello server!\n-Client";

    // if (cli.SendBytes((const uint8_t *)msg, strlen(msg) + 1) < 0)
    //     perror("Failed to send message");

    LOGF("%s", "attempting to receive from server");

    cli->ReceiveReply(&rpy);
    rpy.printReply();
}

// TODO: where should this go?
// member function or here?
int sendCommand(Client &cli) {
    UNUSED(cli);
    return 0;
}
// 