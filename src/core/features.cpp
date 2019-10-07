#include "pingpong/core/features.h"

namespace pingpong {
	std::unordered_map<features::type, const char *> features::names = {
		{features::type::away_notify,       "away-notify"},
		{features::type::batch,             "batch"},
		{features::type::cap_notify,        "cap-notify"},
		{features::type::chghost,           "chghost"},
		{features::type::echo_message,      "echo-message"},
		{features::type::extended_join,     "extended-join"},
		{features::type::invite_notify,     "invite-notify"},
		{features::type::labeled_response,  "labeled-response"},
		{features::type::message_ids,       "message-ids"},
		{features::type::multi_prefix,      "multi-prefix"},
		{features::type::server_time,       "server-time"},
		{features::type::userhost_in_names, "userhost-in-names"},
	};

	std::unordered_map<std::string, features::type> features::types = {
		{"away-notify",       features::type::away_notify},
		{"batch",             features::type::batch},
		{"cap-notify",        features::type::cap_notify},
		{"chghost",           features::type::chghost},
		{"echo-message",      features::type::echo_message},
		{"extended-join",     features::type::extended_join},
		{"invite-notify",     features::type::invite_notify},
		{"labeled-response",  features::type::labeled_response},
		{"message-ids",       features::type::message_ids},
		{"multi-prefix",      features::type::multi_prefix},
		{"server-time",       features::type::server_time},
		{"userhost-in-names", features::type::userhost_in_names},
	};

	std::unordered_set<features::type> features::implemented = {
		features::type::multi_prefix,
		features::type::userhost_in_names,
	};
}
