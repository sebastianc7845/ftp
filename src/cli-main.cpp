#include <iostream>
#include <string>
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

    while(true) {
        bool try_again = true;

        while (try_again) {
            Prompt("Provide username");
            GetInput(cli, input_buf);

            cli.SetUsername(input_buf);

            if (cli.GetUsername().compare("user") != 0) {
                Prompt("No such username. Try again");
                cli.SetUsername("");
            }
            else
                try_again = false;
        }
        
        Prompt("Provide password");
        GetInput(cli, input_buf);

        cli.SetPassword(input_buf);
    }

    // TestProtocol(&cli);

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
    Response rsp;

    LOGF("%s", "attempting to connect to server");
    if (cli->Connect() < 0)
        LOGERRNO;

    LOGF("%s", "Sending a message to the server...");

    const char args[] = "arg arg arg";
    Command cmd("USER", "arg arg arg", strlen(args));

    if (cli->SendCommand(&cmd) < 0)
        LOGERRNO;

    // const char msg[] = "Hello server!\n-Client";

    // if (cli.SendBytes((const uint8_t *)msg, strlen(msg) + 1) < 0)
    //     perror("Failed to send message");

    LOGF("%s", "attempting to receive from server");

    cli->ReceiveResponse(&rsp);
    rsp.printResponse();
}