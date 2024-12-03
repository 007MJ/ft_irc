#ifndef SEVER_HPP
#define SEVER_HPP


class Server{
    public:
        Server();
        ~Server();
        void connection();
        void getport(char *number);
        void getpassword(char *word);

    private:
        long int port;
        char *password;
};



#endif