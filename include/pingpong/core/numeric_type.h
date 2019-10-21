#ifndef PINGPONG_CORE_NUMERIC_TYPE_H_
#define PINGPONG_CORE_NUMERIC_TYPE_H_

namespace pingpong {
	enum class numeric_type: int {
		welcome          =   1, // varies                            -- The post-registration welcome message.
		whois_user       = 311, // "nick user host * :realname"      -- The name-related information of a user.
		whois_server     = 312, // "nick server :server_info"        -- The server a user's conencted to.
		whois_idle       = 317, // "nick \d+( \d+)* :seconds idle.*" -- A user's idle time.
		whois_end        = 318, // "nick :.+"                        -- The end of a whois list.
		channel_modes    = 324, // "#channel +modes"                 -- The modes for a channel.
		channel_creation = 329, // "#channel \d+"                    -- Channel creation time.
		channel_topic    = 332, // "#channel :topic"                 -- Channel topic.
		topic_modified   = 333, // "#channel someone \d+"            -- Who last changed a channel's topic and when.
		names_reply      = 353, // "= #channel :you @someone"        -- A list of people in a channel.
		motd             = 372, // ":- One line of the MOTD"         -- The next line in the message of the day.
		motd_start       = 375, // ":- server Message of the Day -"  -- The server's message of the day.
		whois_host       = 378, // "nick :is connecting from *@* *"  -- Where a user is connecting from.
		whois_modes      = 379, // "nick :is using modes +iwx"       -- A user's modes.
		motd_end         = 376, // ":End of /MOTD command."          -- There's nothing more in the message of the day.
		no_such_nick     = 401, // "someone :No such nick/channel"   -- You tried to message someone who doesn't exist.
		nick_in_use      = 433, // "someone :reason"                 -- The requested nickname is already being used.
	};
}

#endif
