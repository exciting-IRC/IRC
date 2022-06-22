#ifndef UTIL_RETURNCODE_HPP
#define UTIL_RETURNCODE_HPP

namespace util {

/// shamelessly copy-pasted from:
/// https://github.com/inspircd/inspircd/blob/insp3/include/numerics.h
enum returnCode {
  // 클라이언트-서버 전용
  RPL_WELCOME = 1,
  RPL_YOURHOST = 2,
  RPL_CREATED = 3,
  RPL_MYINFO = 4,

  // 명령어 결과값
  RPL_UMODEIS = 221,

  RPL_LUSERCLIENT = 251,
  RPL_LUSEROP = 252,
  RPL_LUSERUNKNOWN = 253,
  RPL_LUSERCHANNELS = 254,
  RPL_LUSERME = 255,

  RPL_ADMINME = 256,
  RPL_ADMINLOC1 = 257,
  RPL_ADMINLOC2 = 258,
  RPL_ADMINEMAIL = 259,

  RPL_LOCALUSERS = 265,
  RPL_GLOBALUSERS = 266,

  RPL_AWAY = 301,
  RPL_USERHOST = 302,
  RPL_ISON = 303,

  RPL_LISTSTART = 321,
  RPL_LIST = 322,
  RPL_LISTEND = 323,

  RPL_CHANNELMODEIS = 324,
  RPL_NOTOPICSET = 331,
  RPL_TOPIC = 332,

  RPL_USERIP = 340,
  RPL_INVITING = 341,
  RPL_VERSION = 351,
  RPL_NAMREPLY = 353,
  RPL_LINKS = 364,
  RPL_ENDOFLINKS = 365,
  RPL_ENDOFNAMES = 366,

  RPL_INFO = 371,
  RPL_ENDOFINFO = 374,
  RPL_MOTD = 372,
  RPL_MOTDSTART = 375,
  RPL_ENDOFMOTD = 376,

  RPL_YOUAREOPER = 381,
  RPL_REHASHING = 382,
  RPL_TIME = 391,

  // 오류 메시지
  ERR_NOSUCHNICK = 401,
  ERR_NOSUCHSERVER = 402,
  ERR_NOSUCHCHANNEL = 403,
  ERR_CANNOTSENDTOCHAN = 404,
  ERR_TOOMANYCHANNELS = 405,
  ERR_WASNOSUCHNICK = 406,
  ERR_NOSUCHSERVICE = 408,  // From RFC 2812.
  ERR_NOORIGIN = 409,

  ERR_NORECIPIENT = 411,
  ERR_NOTEXTTOSEND = 412,

  ERR_UNKNOWNCOMMAND = 421,
  ERR_NOMOTD = 422,

  ERR_NONICKNAMEGIVEN = 431,
  ERR_ERRONEUSNICKNAME = 432,
  ERR_NICKNAMEINUSE = 433,
  ERR_UNAVAILRESOURCE = 437,  // From RFC 2182.

  ERR_USERNOTINCHANNEL = 441,
  ERR_NOTONCHANNEL = 442,
  ERR_USERONCHANNEL = 443,

  ERR_NOTREGISTERED = 451,

  ERR_NEEDMOREPARAMS = 461,
  ERR_ALREADYREGISTERED = 462,
  ERR_YOUREBANNEDCREEP = 465,

  ERR_UNKNOWNMODE = 472,
  ERR_BADCHANNELKEY = 475,
  ERR_BADCHANMASK = 476,
  ERR_INVITEONLYCHAN = 473,
  ERR_CHANNELISFULL = 471,
  ERR_BANNEDFROMCHAN = 474,
  ERR_BANLISTFULL = 478,

  ERR_NOPRIVILEGES = 481,
  ERR_CHANOPRIVSNEEDED = 482,
  ERR_RESTRICTED = 484,

  ERR_NOOPERHOST = 491,

  ERR_UMODEUNKNOWNFLAG = 501,
  ERR_USERSDONTMATCH = 502
};

}  // namespace util

#endif  // UTIL_RETURNCODE_HPP
