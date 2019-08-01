
_Client A connects to server 1 successfuly then disconnects:_

[client A]                       [server 1]
    |                                 |
    |--- PASS ----------------------->|
	|                                 |
	|<--------------- RPL_WELCOME  ---|
	|                                 |
	|--- NICK ----------------------->|
	|                                 |
	|--- USER ----------------------->|
	|                                 |
	|
	...          ...          ...
	|                                 |
	|--- QUIT ----------------------->|


_Client A requests the list of channels, joins a channel and sends a message before leaving the channel:_

[client A]                       [server 1]
	|                                 |
	|--- LIST ----------------------->|
	|                                 |
	|<------------------- RPL_LIST ---|
	|                                 |
	|<---------------- RPL_LISTEND ---|
	|                                 |
	|--- JOIN #born2code ------------>|
	|                                 |
	|<------------------ RPL_TOPIC ---|
	|                                 |
	|<--------------- RPL_NAMREPLY ---|
	|                                 |
	|<--------------- RPL_NAMREPLY ---|
	|                                 |
	|<------------- RPL_ENDOFNAMES ---|
	|                                 |
	|--- PRIVMSG hello #born2code --->|
	|                                 |
	|--- PART #born2code ------------>|
	|                                 |

Augmented BNF representation of a command protocol:

message    =  [ ":" prefix SPACE ] command [ params ] crlf
prefix     =  servername / ( nickname [ [ "!" user ] "@" host ] )
command    =  1*letter / 3digit
params     =  *14( SPACE middle ) [ SPACE ":" trailing ]
		   =/ 14( SPACE middle ) [ SPACE [ ":" ] trailing ]

nospcrlfcl =  %x01-09 / %x0B-0C / %x0E-1F / %x21-39 / %x3B-FF
				; any octet except NUL, CR, LF, " " and ":"
middle     =  nospcrlfcl *( ":" / nospcrlfcl )
trailing   =  *( ":" / " " / nospcrlfcl )

SPACE      =  %x20        ; space character
crlf       =  %x0D %x0A   ; "carriage return" "linefeed"
target     =  nickname / server
msgtarget  =  msgto *( "," msgto )
msgto      =  channel / ( user [ "%" host ] "@" servername )
msgto      =/ ( user "%" host ) / targetmask
msgto      =/ nickname / ( nickname "!" user "@" host )
channel    =  ( "#" / "+" / ( "!" channelid ) / "&" ) chanstring
			[ ":" chanstring ]
servername =  hostname
host       =  hostname / hostaddr
hostname   =  shortname *( "." shortname )
shortname  =  ( letter / digit ) *( letter / digit / "-" )
			*( letter / digit )
			  ; as specified in RFC 1123 [HNAME]
hostaddr   =  ip4addr / ip6addr
ip4addr    =  1*3digit "." 1*3digit "." 1*3digit "." 1*3digit
ip6addr    =  1*hexdigit 7( ":" 1*hexdigit )
ip6addr    =/ "0:0:0:0:0:" ( "0" / "FFFF" ) ":" ip4addr
nickname   =  ( letter / special ) *8( letter / digit / special / "-" )
targetmask =  ( "$" / "#" ) mask
			  ; see details on allowed masks in section 3.3.1
chanstring =  %x01-07 / %x08-09 / %x0B-0C / %x0E-1F / %x21-2B
chanstring =/ %x2D-39 / %x3B-FF
			  ; any octet except NUL, BELL, CR, LF, " ", "," and ":"
channelid  = 5( %x41-5A / digit )   ; 5( A-Z / 0-9 )
user       =  1*( %x01-09 / %x0B-0C / %x0E-1F / %x21-3F / %x41-FF )
			  ; any octet except NUL, CR, LF, " " and "@"
key        =  1*23( %x01-05 / %x07-08 / %x0C / %x0E-1F / %x21-7F )
			  ; any 7-bit US_ASCII character,
			  ; except NUL, CR, LF, FF, h/v TABs, and " "
letter     =  %x41-5A / %x61-7A       ; A-Z / a-z
digit      =  %x30-39                 ; 0-9
hexdigit   =  digit / "A" / "B" / "C" / "D" / "E" / "F"
special    =  %x5B-60 / %x7B-7D
			   ; "[", "]", "\", "`", "_", "^", "{", "|", "}"
