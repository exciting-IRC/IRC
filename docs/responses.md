# 명령어 반환값

## 클라이언트-서버간 연결

Numerics in the range from 001 to 099 are used for client-server
connections only and should never travel between servers.  Replies
generated in the response to commands are found in the range from 200
to 399.

```c
001    RPL_WELCOME
      "Welcome to the Internet Relay Network
        <nick>!<user>@<host>"
002    RPL_YOURHOST
      "Your host is <servername>, running version <ver>"
003    RPL_CREATED
      "This server was created <date>"
004    RPL_MYINFO
      "<servername> <version> <available user modes>
        <available channel modes>"

  - The server sends Replies 001 to 004 to a user upon
    successful registration.

005    RPL_BOUNCE
      "Try server <server name>, port <port number>"

  - Sent by the server to a user to suggest an alternative
    server.  This is often used when the connection is
    refused because the server is already full.

302    RPL_USERHOST
      ":*1<reply> *( " " <reply> )"

  - Reply format used by USERHOST to list replies to
    the query list.  The reply string is composed as
    follows:

    reply = nickname [ "*" ] "=" ( "+" / "-" ) hostname

    The '*' indicates whether the client has registered
    as an Operator.  The '-' or '+' characters represent
    whether the client has set an AWAY message or not
    respectively.

303    RPL_ISON
      ":*1<nick> *( " " <nick> )"

  - Reply format used by ISON to list replies to the
    query list.

301    RPL_AWAY
      "<nick> :<away message>"
305    RPL_UNAWAY
      ":You are no longer marked as being away"
306    RPL_NOWAWAY
      ":You have been marked as being away"

  - These replies are used with the AWAY command (if
    allowed).  RPL_AWAY is sent to any client sending a
    PRIVMSG to a client which is away.  RPL_AWAY is only
    sent by the server to which the client is connected.
    Replies RPL_UNAWAY and RPL_NOWAWAY are sent when the
    client removes and sets an AWAY message.

311    RPL_WHOISUSER
      "<nick> <user> <host> * :<real name>"
312    RPL_WHOISSERVER
      "<nick> <server> :<server info>"
313    RPL_WHOISOPERATOR
      "<nick> :is an IRC operator"

317    RPL_WHOISIDLE
      "<nick> <integer> :seconds idle"
318    RPL_ENDOFWHOIS
      "<nick> :End of WHOIS list"
319    RPL_WHOISCHANNELS
      "<nick> :*( ( "@" / "+" ) <channel> " " )"

  - Replies 311 - 313, 317 - 319 are all replies
    generated in response to a WHOIS message.  Given that
    there are enough parameters present, the answering
    server MUST either formulate a reply out of the above
    numerics (if the query nick is found) or return an
    error reply.  The '*' in RPL_WHOISUSER is there as
    the literal character and not as a wild card.  For
    each reply set, only RPL_WHOISCHANNELS may appear
    more than once (for long lists of channel names).
    The '@' and '+' characters next to the channel name
    indicate whether a client is a channel operator or
    has been granted permission to speak on a moderated
    channel.  The RPL_ENDOFWHOIS reply is used to mark
    the end of processing a WHOIS message.

314    RPL_WHOWASUSER
      "<nick> <user> <host> * :<real name>"
369    RPL_ENDOFWHOWAS
      "<nick> :End of WHOWAS"

  - When replying to a WHOWAS message, a server MUST use
    the replies RPL_WHOWASUSER, RPL_WHOISSERVER or
    ERR_WASNOSUCHNICK for each nickname in the presented
    list.  At the end of all reply batches, there MUST
    be RPL_ENDOFWHOWAS (even if there was only one reply
    and it was an error).

321    RPL_LISTSTART
      Obsolete. Not used.

322    RPL_LIST
      "<channel> <# visible> :<topic>"
323    RPL_LISTEND
      ":End of LIST"

  - Replies RPL_LIST, RPL_LISTEND mark the actual replies
    with data and end of the server's response to a LIST
    command.  If there are no channels available to return,
    only the end reply MUST be sent.

325    RPL_UNIQOPIS
      "<channel> <nickname>"

324    RPL_CHANNELMODEIS
      "<channel> <mode> <mode params>"

331    RPL_NOTOPIC
      "<channel> :No topic is set"
332    RPL_TOPIC
      "<channel> :<topic>"

  - When sending a TOPIC message to determine the
    channel topic, one of two replies is sent.  If
    the topic is set, RPL_TOPIC is sent back else
    RPL_NOTOPIC.

341    RPL_INVITING
      "<channel> <nick>"

  - Returned by the server to indicate that the
    attempted INVITE message was successful and is
    being passed onto the end client.

342    RPL_SUMMONING
      "<user> :Summoning user to IRC"

  - Returned by a server answering a SUMMON message to
    indicate that it is summoning that user.

346    RPL_INVITELIST
      "<channel> <invitemask>"
347    RPL_ENDOFINVITELIST
      "<channel> :End of channel invite list"

  - When listing the 'invitations masks' for a given channel,
    a server is required to send the list back using the
    RPL_INVITELIST and RPL_ENDOFINVITELIST messages.  A
    separate RPL_INVITELIST is sent for each active mask.
    After the masks have been listed (or if none present) a
    RPL_ENDOFINVITELIST MUST be sent.

348    RPL_EXCEPTLIST
      "<channel> <exceptionmask>"
349    RPL_ENDOFEXCEPTLIST
      "<channel> :End of channel exception list"

  - When listing the 'exception masks' for a given channel,
    a server is required to send the list back using the
    RPL_EXCEPTLIST and RPL_ENDOFEXCEPTLIST messages.  A
    separate RPL_EXCEPTLIST is sent for each active mask.
    After the masks have been listed (or if none present)
    a RPL_ENDOFEXCEPTLIST MUST be sent.

351    RPL_VERSION
      "<version>.<debuglevel> <server> :<comments>"

  - Reply by the server showing its version details.
    The <version> is the version of the software being
    used (including any patchlevel revisions) and the
    <debuglevel> is used to indicate if the server is
    running in "debug mode".

    The "comments" field may contain any comments about
    the version or further version details.

352    RPL_WHOREPLY
      "<channel> <user> <host> <server> <nick>
      ( "H" / "G" > ["*"] [ ( "@" / "+" ) ]
      :<hopcount> <real name>"

315    RPL_ENDOFWHO
      "<name> :End of WHO list"

  - The RPL_WHOREPLY and RPL_ENDOFWHO pair are used
    to answer a WHO message.  The RPL_WHOREPLY is only
    sent if there is an appropriate match to the WHO
    query.  If there is a list of parameters supplied
    with a WHO message, a RPL_ENDOFWHO MUST be sent
    after processing each list item with <name> being
    the item.

353    RPL_NAMREPLY
      "( "=" / "*" / "@" ) <channel>
        :[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> )
  - "@" is used for secret channels, "*" for private
    channels, and "=" for others (public channels).

366    RPL_ENDOFNAMES
      "<channel> :End of NAMES list"

  - To reply to a NAMES message, a reply pair consisting
    of RPL_NAMREPLY and RPL_ENDOFNAMES is sent by the
    server back to the client.  If there is no channel
    found as in the query, then only RPL_ENDOFNAMES is

    returned.  The exception to this is when a NAMES
    message is sent with no parameters and all visible
    channels and contents are sent back in a series of
    RPL_NAMEREPLY messages with a RPL_ENDOFNAMES to mark
    the end.

364    RPL_LINKS
      "<mask> <server> :<hopcount> <server info>"
365    RPL_ENDOFLINKS
      "<mask> :End of LINKS list"

  - In replying to the LINKS message, a server MUST send
    replies back using the RPL_LINKS numeric and mark the
    end of the list using an RPL_ENDOFLINKS reply.

367    RPL_BANLIST
      "<channel> <banmask>"
368    RPL_ENDOFBANLIST
      "<channel> :End of channel ban list"

  - When listing the active 'bans' for a given channel,
    a server is required to send the list back using the
    RPL_BANLIST and RPL_ENDOFBANLIST messages.  A separate
    RPL_BANLIST is sent for each active banmask.  After the
    banmasks have been listed (or if none present) a
    RPL_ENDOFBANLIST MUST be sent.

371    RPL_INFO
      ":<string>"
374    RPL_ENDOFINFO
      ":End of INFO list"

  - A server responding to an INFO message is required to
    send all its 'info' in a series of RPL_INFO messages
    with a RPL_ENDOFINFO reply to indicate the end of the
    replies.

375    RPL_MOTDSTART
      ":- <server> Message of the day - "
372    RPL_MOTD
      ":- <text>"
376    RPL_ENDOFMOTD
      ":End of MOTD command"

  - When responding to the MOTD message and the MOTD file
    is found, the file is displayed line by line, with
    each line no longer than 80 characters, using

    RPL_MOTD format replies.  These MUST be surrounded
    by a RPL_MOTDSTART (before the RPL_MOTDs) and an
    RPL_ENDOFMOTD (after).

381    RPL_YOUREOPER
      ":You are now an IRC operator"

  - RPL_YOUREOPER is sent back to a client which has
    just successfully issued an OPER message and gained
    operator status.

382    RPL_REHASHING
      "<config file> :Rehashing"

  - If the REHASH option is used and an operator sends
    a REHASH message, an RPL_REHASHING is sent back to
    the operator.

383    RPL_YOURESERVICE
      "You are service <servicename>"

  - Sent by the server to a service upon successful
    registration.

391    RPL_TIME
      "<server> :<string showing server's local time>"

  - When replying to the TIME message, a server MUST send
    the reply using the RPL_TIME format above.  The string
    showing the time need only contain the correct day and
    time there.  There is no further requirement for the
    time string.

392    RPL_USERSSTART
      ":UserID   Terminal  Host"
393    RPL_USERS
      ":<username> <ttyline> <hostname>"
394    RPL_ENDOFUSERS
      ":End of users"
395    RPL_NOUSERS
      ":Nobody logged in"

  - If the USERS message is handled by a server, the
    replies RPL_USERSTART, RPL_USERS, RPL_ENDOFUSERS and
    RPL_NOUSERS are used.  RPL_USERSSTART MUST be sent
    first, following by either a sequence of RPL_USERS
    or a single RPL_NOUSER.  Following this is
    RPL_ENDOFUSERS.

200    RPL_TRACELINK
      "Link <version & debug level> <destination>
        <next server> V<protocol version>
        <link uptime in seconds> <backstream sendq>
        <upstream sendq>"
201    RPL_TRACECONNECTING
      "Try. <class> <server>"
202    RPL_TRACEHANDSHAKE
      "H.S. <class> <server>"
203    RPL_TRACEUNKNOWN
      "???? <class> [<client IP address in dot form>]"
204    RPL_TRACEOPERATOR
      "Oper <class> <nick>"
205    RPL_TRACEUSER
      "User <class> <nick>"
206    RPL_TRACESERVER
      "Serv <class> <int>S <int>C <server>
        <nick!user|*!*>@<host|server> V<protocol version>"
207    RPL_TRACESERVICE
      "Service <class> <name> <type> <active type>"
208    RPL_TRACENEWTYPE
      "<newtype> 0 <client name>"
209    RPL_TRACECLASS
      "Class <class> <count>"
210    RPL_TRACERECONNECT
      Unused.
261    RPL_TRACELOG
      "File <logfile> <debug level>"
262    RPL_TRACEEND
      "<server name> <version & debug level> :End of TRACE"

  - The RPL_TRACE* are all returned by the server in
    response to the TRACE message.  How many are
    returned is dependent on the TRACE message and
    whether it was sent by an operator or not.  There
    is no predefined order for which occurs first.
    Replies RPL_TRACEUNKNOWN, RPL_TRACECONNECTING and
    RPL_TRACEHANDSHAKE are all used for connections
    which have not been fully established and are either
    unknown, still attempting to connect or in the
    process of completing the 'server handshake'.
    RPL_TRACELINK is sent by any server which handles
    a TRACE message and has to pass it on to another
    server.  The list of RPL_TRACELINKs sent in
    response to a TRACE command traversing the IRC
    network should reflect the actual connectivity of
    the servers themselves along that path.

    RPL_TRACENEWTYPE is to be used for any connection
    which does not fit in the other categories but is
    being displayed anyway.
    RPL_TRACEEND is sent to indicate the end of the list.

211    RPL_STATSLINKINFO
      "<linkname> <sendq> <sent messages>
        <sent Kbytes> <received messages>
        <received Kbytes> <time open>"

  - reports statistics on a connection.  <linkname>
    identifies the particular connection, <sendq> is
    the amount of data that is queued and waiting to be
    sent <sent messages> the number of messages sent,
    and <sent Kbytes> the amount of data sent, in
    Kbytes. <received messages> and <received Kbytes>
    are the equivalent of <sent messages> and <sent
    Kbytes> for received data, respectively.  <time
    open> indicates how long ago the connection was
    opened, in seconds.

212    RPL_STATSCOMMANDS
      "<command> <count> <byte count> <remote count>"

  - reports statistics on commands usage.

219    RPL_ENDOFSTATS
      "<stats letter> :End of STATS report"

242    RPL_STATSUPTIME
      ":Server Up %d days %d:%02d:%02d"

  - reports the server uptime.

243    RPL_STATSOLINE
      "O <hostmask> * <name>"

  - reports the allowed hosts from where user may become IRC
    operators.

221    RPL_UMODEIS
      "<user mode string>"

  - To answer a query about a client's own mode,
    RPL_UMODEIS is sent back.

234    RPL_SERVLIST
      "<name> <server> <mask> <type> <hopcount> <info>"

235    RPL_SERVLISTEND
      "<mask> <type> :End of service listing"

  - When listing services in reply to a SERVLIST message,
    a server is required to send the list back using the
    RPL_SERVLIST and RPL_SERVLISTEND messages.  A separate
    RPL_SERVLIST is sent for each service.  After the
    services have been listed (or if none present) a
    RPL_SERVLISTEND MUST be sent.

251    RPL_LUSERCLIENT
      ":There are <integer> users and <integer>
        services on <integer> servers"
252    RPL_LUSEROP
      "<integer> :operator(s) online"
253    RPL_LUSERUNKNOWN
      "<integer> :unknown connection(s)"
254    RPL_LUSERCHANNELS
      "<integer> :channels formed"
255    RPL_LUSERME
      ":I have <integer> clients and <integer>
        servers"

  - In processing an LUSERS message, the server
    sends a set of replies from RPL_LUSERCLIENT,
    RPL_LUSEROP, RPL_USERUNKNOWN,
    RPL_LUSERCHANNELS and RPL_LUSERME.  When
    replying, a server MUST send back
    RPL_LUSERCLIENT and RPL_LUSERME.  The other
    replies are only sent back if a non-zero count
    is found for them.

256    RPL_ADMINME
      "<server> :Administrative info"
257    RPL_ADMINLOC1
      ":<admin info>"
258    RPL_ADMINLOC2
      ":<admin info>"
259    RPL_ADMINEMAIL
      ":<admin info>"

  - When replying to an ADMIN message, a server
    is expected to use replies RPL_ADMINME
    through to RPL_ADMINEMAIL and provide a text
    message with each.  For RPL_ADMINLOC1 a
    description of what city, state and country
    the server is in is expected, followed by
    details of the institution (RPL_ADMINLOC2)

    and finally the administrative contact for the
    server (an email address here is REQUIRED)
    in RPL_ADMINEMAIL.

263    RPL_TRYAGAIN
      "<command> :Please wait a while and try again."

  - When a server drops a command without processing it,
    it MUST use the reply RPL_TRYAGAIN to inform the
    originating client.
```

## 에러 반환값 (400 ~ 599)

```c
401    ERR_NOSUCHNICK
      "<nickname> :No such nick/channel"

  - Used to indicate the nickname parameter supplied to a
    command is currently unused.

402    ERR_NOSUCHSERVER
      "<server name> :No such server"

  - Used to indicate the server name given currently
    does not exist.

403    ERR_NOSUCHCHANNEL
      "<channel name> :No such channel"

  - Used to indicate the given channel name is invalid.

404    ERR_CANNOTSENDTOCHAN
      "<channel name> :Cannot send to channel"

  - Sent to a user who is either (a) not on a channel
    which is mode +n or (b) not a chanop (or mode +v) on
    a channel which has mode +m set or where the user is
    banned and is trying to send a PRIVMSG message to
    that channel.

405    ERR_TOOMANYCHANNELS
      "<channel name> :You have joined too many channels"

  - Sent to a user when they have joined the maximum
    number of allowed channels and they try to join
    another channel.

406    ERR_WASNOSUCHNICK
      "<nickname> :There was no such nickname"

  - Returned by WHOWAS to indicate there is no history
    information for that nickname.

407    ERR_TOOMANYTARGETS
      "<target> :<error code> recipients. <abort message>"

  - Returned to a client which is attempting to send a
    PRIVMSG/NOTICE using the user@host destination format
    and for a user@host which has several occurrences.

  - Returned to a client which trying to send a
    PRIVMSG/NOTICE to too many recipients.

  - Returned to a client which is attempting to JOIN a safe
    channel using the shortname when there are more than one
    such channel.

408    ERR_NOSUCHSERVICE
      "<service name> :No such service"

  - Returned to a client which is attempting to send a SQUERY
    to a service which does not exist.

409    ERR_NOORIGIN
      ":No origin specified"

  - PING or PONG message missing the originator parameter.

411    ERR_NORECIPIENT
      ":No recipient given (<command>)"
412    ERR_NOTEXTTOSEND
      ":No text to send"
413    ERR_NOTOPLEVEL
      "<mask> :No toplevel domain specified"
414    ERR_WILDTOPLEVEL
      "<mask> :Wildcard in toplevel domain"
415    ERR_BADMASK
      "<mask> :Bad Server/host mask"

  - 412 - 415 are returned by PRIVMSG to indicate that
    the message wasn't delivered for some reason.
    ERR_NOTOPLEVEL and ERR_WILDTOPLEVEL are errors that
    are returned when an invalid use of
    "PRIVMSG $<server>" or "PRIVMSG #<host>" is attempted.

421    ERR_UNKNOWNCOMMAND
      "<command> :Unknown command"

  - Returned to a registered client to indicate that the
    command sent is unknown by the server.

422    ERR_NOMOTD
      ":MOTD File is missing"

  - Server's MOTD file could not be opened by the server.

423    ERR_NOADMININFO
      "<server> :No administrative info available"

  - Returned by a server in response to an ADMIN message
    when there is an error in finding the appropriate
    information.

424    ERR_FILEERROR
      ":File error doing <file op> on <file>"

  - Generic error message used to report a failed file
    operation during the processing of a message.

431    ERR_NONICKNAMEGIVEN
      ":No nickname given"

  - Returned when a nickname parameter expected for a
    command and isn't found.

432    ERR_ERRONEUSNICKNAME
      "<nick> :Erroneous nickname"

  - Returned after receiving a NICK message which contains
    characters which do not fall in the defined set.  See
    section 2.3.1 for details on valid nicknames.

433    ERR_NICKNAMEINUSE
      "<nick> :Nickname is already in use"

  - Returned when a NICK message is processed that results
    in an attempt to change to a currently existing
    nickname.

436    ERR_NICKCOLLISION
      "<nick> :Nickname collision KILL from <user>@<host>"

  - Returned by a server to a client when it detects a
    nickname collision (registered of a NICK that
    already exists by another server).

437    ERR_UNAVAILRESOURCE
      "<nick/channel> :Nick/channel is temporarily unavailable"

  - Returned by a server to a user trying to join a channel
    currently blocked by the channel delay mechanism.

  - Returned by a server to a user trying to change nickname
    when the desired nickname is blocked by the nick delay
    mechanism.

441    ERR_USERNOTINCHANNEL
      "<nick> <channel> :They aren't on that channel"

  - Returned by the server to indicate that the target
    user of the command is not on the given channel.

442    ERR_NOTONCHANNEL
      "<channel> :You're not on that channel"

  - Returned by the server whenever a client tries to
    perform a channel affecting command for which the
    client isn't a member.

443    ERR_USERONCHANNEL
      "<user> <channel> :is already on channel"

  - Returned when a client tries to invite a user to a
    channel they are already on.

444    ERR_NOLOGIN
      "<user> :User not logged in"

  - Returned by the summon after a SUMMON command for a
    user was unable to be performed since they were not
    logged in.

445    ERR_SUMMONDISABLED
      ":SUMMON has been disabled"

  - Returned as a response to the SUMMON command.  MUST be
    returned by any server which doesn't implement it.

446    ERR_USERSDISABLED
      ":USERS has been disabled"

  - Returned as a response to the USERS command.  MUST be
    returned by any server which does not implement it.

451    ERR_NOTREGISTERED
      ":You have not registered"

  - Returned by the server to indicate that the client
    MUST be registered before the server will allow it
    to be parsed in detail.

461    ERR_NEEDMOREPARAMS
      "<command> :Not enough parameters"

  - Returned by the server by numerous commands to
    indicate to the client that it didn't supply enough
    parameters.

462    ERR_ALREADYREGISTRED
      ":Unauthorized command (already registered)"

  - Returned by the server to any link which tries to
    change part of the registered details (such as
    password or user details from second USER message).

463    ERR_NOPERMFORHOST
      ":Your host isn't among the privileged"

  - Returned to a client which attempts to register with
    a server which does not been setup to allow
    connections from the host the attempted connection
    is tried.

464    ERR_PASSWDMISMATCH
      ":Password incorrect"

  - Returned to indicate a failed attempt at registering
    a connection for which a password was required and
    was either not given or incorrect.

465    ERR_YOUREBANNEDCREEP
      ":You are banned from this server"

  - Returned after an attempt to connect and register
    yourself with a server which has been setup to
    explicitly deny connections to you.

466    ERR_YOUWILLBEBANNED

  - Sent by a server to a user to inform that access to the
    server will soon be denied.

467    ERR_KEYSET
      "<channel> :Channel key already set"
471    ERR_CHANNELISFULL
      "<channel> :Cannot join channel (+l)"
472    ERR_UNKNOWNMODE
      "<char> :is unknown mode char to me for <channel>"
473    ERR_INVITEONLYCHAN
      "<channel> :Cannot join channel (+i)"
474    ERR_BANNEDFROMCHAN
      "<channel> :Cannot join channel (+b)"
475    ERR_BADCHANNELKEY
      "<channel> :Cannot join channel (+k)"
476    ERR_BADCHANMASK
      "<channel> :Bad Channel Mask"
477    ERR_NOCHANMODES
      "<channel> :Channel doesn't support modes"
478    ERR_BANLISTFULL
      "<channel> <char> :Channel list is full"

481    ERR_NOPRIVILEGES
      ":Permission Denied- You're not an IRC operator"

  - Any command requiring operator privileges to operate
    MUST return this error to indicate the attempt was
    unsuccessful.

482    ERR_CHANOPRIVSNEEDED
      "<channel> :You're not channel operator"

  - Any command requiring 'chanop' privileges (such as
    MODE messages) MUST return this error if the client
    making the attempt is not a chanop on the specified
    channel.

483    ERR_CANTKILLSERVER
      ":You can't kill a server!"

  - Any attempts to use the KILL command on a server
    are to be refused and this error returned directly
    to the client.

484    ERR_RESTRICTED
      ":Your connection is restricted!"

  - Sent by the server to a user upon connection to indicate
    the restricted nature of the connection (user mode "+r").

485    ERR_UNIQOPPRIVSNEEDED
      ":You're not the original channel operator"

  - Any MODE requiring "channel creator" privileges MUST
    return this error if the client making the attempt is not
    a chanop on the specified channel.

491    ERR_NOOPERHOST
      ":No O-lines for your host"

  - If a client sends an OPER message and the server has
    not been configured to allow connections from the
    client's host as an operator, this error MUST be
    returned.

501    ERR_UMODEUNKNOWNFLAG
      ":Unknown MODE flag"

  - Returned by the server to indicate that a MODE
    message was sent with a nickname parameter and that
    the a mode flag sent was not recognized.

502    ERR_USERSDONTMATCH
      ":Cannot change mode for other users"

  - Error sent to any user trying to view or change the
    user mode for a user other than themselves.
```

## 쓰이지는 않지만 예약된 반환값

1. 지금 안 쓰이거나
2. 나중에 쓰려고 하는 경우
3. 쓰이고는 있지만 일반적이지 않은 '기능'으로 쓰이는 경우

```c
231    RPL_SERVICEINFO     232  RPL_ENDOFSERVICES
233    RPL_SERVICE
300    RPL_NONE            316  RPL_WHOISCHANOP
361    RPL_KILLDONE        362  RPL_CLOSING
363    RPL_CLOSEEND        373  RPL_INFOSTART
384    RPL_MYPORTIS

213    RPL_STATSCLINE      214  RPL_STATSNLINE
215    RPL_STATSILINE      216  RPL_STATSKLINE
217    RPL_STATSQLINE      218  RPL_STATSYLINE
240    RPL_STATSVLINE      241  RPL_STATSLLINE
244    RPL_STATSHLINE      244  RPL_STATSSLINE
246    RPL_STATSPING       247  RPL_STATSBLINE
250    RPL_STATSDLINE

492    ERR_NOSERVICEHOST
```
