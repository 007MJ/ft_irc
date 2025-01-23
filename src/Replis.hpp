#include "../includes/utils.hpp"

class Replis
{
    public:
        Replis(/* args */);
        ~Replis();
        std::string ERR_NICKNAMEINUSE   (std::string nick) { return ("433 " + nick + ":Nickname is already in use\r\n");}
        std::string ERR_NICKCOLLISION   () { return " 436 <nick> :Nickname collision KILL from <user>@<host>\r\n";}
        std::string ERR_UNAVAILRESOURCE   () {return "437  <nick/channel> :Nick/channel is temporarily unavailable\r\n";}
        std::string ERR_RESTRICTED   () {return " 484 :Your connection is restricted!\r\n";}
        std::string ERR_NEEDMOREPARAMS   () { return " 461 <command> :Not enough parameters\r\n";}
        std::string ERR_BANNEDFROMCHAN   () { return "<channel> :Cannot join channel (+b)\r\n";}
        std::string ERR_INVITEONLYCHAN    () {return "473 <channel> :Cannot join channel (+i)\r\n" ; }
        std::string ERR_BADCHANNELKEY    () { return " 475 <channel> :Cannot join channel (+k)\r\n";}
        std::string ERR_CHANNELISFULL    () {return "471 <channel> :Cannot join channel (+l)\r\n";}
        std::string ERR_BADCHANMASK    () {return " 476 <channel> :Bad Channel Mask\r\n";}
        std::string ERR_NOSUCHCHANNEL     () {return "403 <channel name> :No such channel\r\n";}
        std::string ERR_TOOMANYCHANNELS     () {return "405 <channel name> :You have joined too many channels\r\n";}
        std::string ERR_TOOMANYTARGETS    () {return "407 <target> :<error code> recipients. <abort message>\r\n";}
        std::string RPL_TOPIC    () {return "332  <channel> :<topic>\r\n";}
        std::string ERR_USERNOTINCHANNEL    () {return "441 <nick> <channel> :They aren't on that channel\r\n";}
        std::string ERR_NOTONCHANNEL   () {return "442  <channel> :You're not on that channel\r\n";}
        std::string ERR_NOSUCHNICK   () {return "401 <nickname> :No such nick/channel\r\n";}
        std::string ERR_USERONCHANNEL   () {return "443  <user> <channel> :is already on channel\r\n";}
        std::string RPL_INVITING     () {return "341  <channel> <nick>\r\n";}
        std::string RPL_AWAY    () {return "301 <nick> :<away message>\r\n";}
        std::string ERR_KEYSET   () {return "476 <channel> :Channel key already set\r\n"; }
        std::string ERR_NOCHANMODES  () {return "477 <channel> :Channel doesn't support modes\r\n";}
        std::string ERR_CHANOPRIVSNEEDED () {return "482 <channel> :You're not channel operator\r\n";}
        std::string ERR_UNKNOWNMODE () {return "472 <char> :is unknown mode char to me for <channel>\r\n";}
        std::string RPL_CHANNELMODEIS () {return "<channel> <mode> <mode params>\r\n";}
        std::string RPL_BANLIST() {return "367 <channel> <banmask>\r\n";}
        std::string RPL_ENDOFBANLIST() {return "368 <channel> :End of channel ban list\r\n";}
        std::string RPL_EXCEPTLIST  () {return "348 <channel> <exceptionmask>\r\n";}
        std::string RPL_ENDOFEXCEPTLIST  () {return "349 <channel> :End of channel exception list\r\n";} 
        std::string RPL_INVITELIST  () {return "346 <channel> <invitemask>\r\n";}
        std::string RPL_ENDOFINVITELIST () {return "347  <channel> :End of channel invite list\r\n";}
        std::string RPL_UNIQOPIS () {return "325 <channel> <nickname>\r\n";}

        std::string RPL_ENDOFNAMES(std::string nickname, std::string channel) {
         return ":irc.example.com 366 " + nickname + " " + channel + " :End of /NAMES list\r\n";
        }


        std::string NOTIFCHANNEL(std::string nickname, std::string username, std::string channel) {
        return ":" + nickname + "!" + username + "@hostname JOIN " + channel + "\r\n";
        }

        std::string RPL_NOTOPIC(std::string nickname, std::string channel) {
        return ":irc.example.com 331 " + nickname + " " + channel + " :No topic is set\r\n";
        }

        std::string RPL_TOPIC(std::string nickname, std::string channel, std::string topic) {
        return ":irc.example.com 332 " + nickname + " " + channel + " :" + topic + "\r\n";
        }

        std::string RPL_NOPRIVILEGES(std::string nickname) {
        return ":irc.example.com 481 " + nickname + " :Permission Denied- You're not a superuser\r\n";
        }

        std::string RPL_INVITEONLY(std::string nickname, std::string channel) {
        return ":irc.example.com 473 " + nickname + " " + channel + " :Channel is not invite-only.\r\n";
        }


        // std::string ERR_INVITEONLYCHAN (std::string nickname,  std::string username , std::string channel) { return ("server 473 <nickname> <channel> :Cannot join channel (+i) \r\n");}
    };

Replis::Replis(/* args */)
{
}

Replis::~Replis()
{
}
