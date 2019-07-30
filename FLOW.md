
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


_Client A joins requests the list of channels, joins one and sends a message before leaving:_

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
