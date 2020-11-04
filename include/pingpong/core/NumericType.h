#ifndef PINGPONG_CORE_NUMERICTYPE_H_
#define PINGPONG_CORE_NUMERICTYPE_H_

namespace PingPong {
	enum class NumericType: int {
		Welcome         =   1, // varies                            -- The post-registration welcome message.
		ISupport        =   5, // "KEY=value" ..., generally        -- Displays various server parameters.
		WhoisUser       = 311, // "nick user host * :realname"      -- The name-related information of a user.
		WhoisServer     = 312, // "nick server :server_info"        -- The server a user's conencted to.
		WhoisIdle       = 317, // "nick \d+( \d+)* :seconds idle.*" -- A user's idle time.
		WhoisEnd        = 318, // "nick :.+"                        -- The end of a whois list.
		ChannelModes    = 324, // "#channel +modes"                 -- The modes for a channel.
		ChannelCreation = 329, // "#channel \d+"                    -- Channel creation time.
		ChannelTopic    = 332, // "#channel :topic"                 -- Channel topic.
		TopicModified   = 333, // "#channel someone \d+"            -- Who last changed a channel's topic and when.
		NamesReply      = 353, // "= #channel :you @someone"        -- A list of people in a channel.
		Motd            = 372, // ":- One line of the MOTD"         -- The next line in the message of the day.
		MotdStart       = 375, // ":- server Message of the Day -"  -- The server's message of the day.
		WhoisHost       = 378, // "nick :is connecting from *@* *"  -- Where a user is connecting from.
		WhoisModes      = 379, // "nick :is using modes +iwx"       -- A user's modes.
		MotdEnd         = 376, // ":End of /MOTD command."          -- There's nothing more in the message of the day.
		NoSuchNick      = 401, // "someone :No such nick/channel"   -- You tried to message someone who doesn't exist.
		NickInUse       = 433, // "someone :reason"                 -- The requested nickname is already being used.
	};
}

#endif
