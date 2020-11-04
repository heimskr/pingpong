#include "pingpong/core/Features.h"

namespace PingPong {
	std::unordered_map<Features::Type, const char *> Features::names = {
		{Features::Type::AwayNotify,      "away-notify"},
		{Features::Type::Batch,           "batch"},
		{Features::Type::CapNotify,       "cap-notify"},
		{Features::Type::ChgHost,         "chghost"},
		{Features::Type::EchoMessage,     "echo-message"},
		{Features::Type::ExtendedJoin,    "extended-join"},
		{Features::Type::InviteNotify,    "invite-notify"},
		{Features::Type::LabeledResponse, "labeled-response"},
		{Features::Type::MessageIDs,      "message-ids"},
		{Features::Type::MultiPrefix,     "multi-prefix"},
		{Features::Type::ServerTime,      "server-time"},
		{Features::Type::UserhostInNames, "userhost-in-names"},
	};

	std::unordered_map<std::string, Features::Type> Features::types = {
		{"away-notify",       Features::Type::AwayNotify},
		{"batch",             Features::Type::Batch},
		{"cap-notify",        Features::Type::CapNotify},
		{"chghost",           Features::Type::ChgHost},
		{"echo-message",      Features::Type::EchoMessage},
		{"extended-join",     Features::Type::ExtendedJoin},
		{"invite-notify",     Features::Type::InviteNotify},
		{"labeled-response",  Features::Type::LabeledResponse},
		{"message-ids",       Features::Type::MessageIDs},
		{"multi-prefix",      Features::Type::MultiPrefix},
		{"server-time",       Features::Type::ServerTime},
		{"userhost-in-names", Features::Type::UserhostInNames},
	};

	std::unordered_set<Features::Type> Features::implemented = {
		Features::Type::ExtendedJoin,
		Features::Type::MultiPrefix,
		Features::Type::UserhostInNames,
	};
}
