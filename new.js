#!/usr/bin/env node
const fs = require("fs");

const yikes = (...a) => { console.error(...a); process.exit(1); };

const [,, ...args] = process.argv;
if (args.length < 2 || !args[1]) {
	yikes("Usage: new.js [type] [name]");
}

const [type, name] = args;

if (!name.match(/^[\w_\d]+$/i)) {
	yikes("Invalid name:", name);
}

let allDir, sourcepath, headerpath, sourcetext, headertext;

if (type.match(/^(com(mands?)?|cmd)$/i)) {
	sourcepath = `commands/${name}.cpp`;
	headerpath = `include/commands/${name}.h`;
	const cls = `${name}_command`;

	headertext = `
#ifndef COMMANDS_${name.toUpperCase()}_H_
#define COMMANDS_${name.toUpperCase()}_H_

#include <string>

#include "command.h"

namespace pingpong {
	class ${cls}: public command {
		public:
			using command::command;


			operator std::string() const override;
	};
}

#endif
`.substr(1);

	sourcetext = `
#include <string>

#include "commands/${name}.h"

namespace pingpong {
	${cls}::operator std::string() const {
		
	}
}
`.substr(1);

	updateModule(allDir = "commands", name);
} else if (type.match(/^(mes(sages?)?|msg|m)(-\w+)?$/i)) {
	const base = type.match(/-/)? type.replace(/^.+-/, "") : null;
	const parent = base? `${base}_message` : "message";

	sourcepath = `messages/${name}.cpp`;
	headerpath = `include/messages/${name}.h`;
	const cls = `${name}_message`;

	headertext = `
#ifndef MESSAGES_${name.toUpperCase()}_H_
#define MESSAGES_${name.toUpperCase()}_H_

#include <string>

#include "message.h"
${base? `#include "${base}.h"\n` : ""}
namespace pingpong {
	class ${cls}: public ${parent} {
		public:
			using ${parent}::${parent};
			static constexpr auto get_name = []() -> std::string { return "${args[2] || name.toUpperCase()}"; };

			operator std::string() const override;
	};
}

#endif
`.substr(1);

	sourcetext = `
#include <string>

#include "messages/${name}.h"

namespace pingpong {
	${cls}::operator std::string() const {
		
	}
}
`.substr(1);

	const ircText = fs.readFileSync("core/irc.cpp", "utf8");
	if (ircText.indexOf(`add_ctor<${cls}>`) == -1) {
		fs.writeFileSync("core/irc.cpp",
			ircText.replace(/(void irc::init_messages\(\) {)/, `$1\n\t\tmessage::add_ctor<${cls}>();`));
	}

	updateModule(allDir = "messages", name);
} else if (type.match(/^l(ib(rary)?)?$/i)) {
	sourcepath = `lib/${name}.cpp`;
	headerpath = `include/lib/${name}.h`;

	headertext = `
#ifndef LIB_${name.toUpperCase()}_H_
#define LIB_${name.toUpperCase()}_H_

namespace pingpong {
	
}

#endif
`.substr(1);

	sourcetext = `
#include "lib/${name}.h"

namespace pingpong {

}
`.substr(1);

	updateModule("lib", name);
} else if (type.match(/^core$/i)) {
	sourcepath = `core/${name}.cpp`;
	headerpath = `include/core/${name}.h`;

	headertext = `
#ifndef CORE_${name.toUpperCase()}_H_
#define CORE_${name.toUpperCase()}_H_

namespace pingpong {
	
}

#endif
`.substr(1);

	sourcetext = `
#include "core/${name}.h"

namespace pingpong {

}
`.substr(1);

	updateModule(allDir = "core", name);
} else {
	console.error("Unknown type:", type);
	yikes(`Expected "command" | "core" | "lib" | "message"`);
}

fs.writeFileSync(sourcepath, sourcetext);
fs.writeFileSync(headerpath, headertext);

if (allDir) {
	const allText = fs.readFileSync(`include/${allDir}/all.h`, "utf8");
	if (allText.indexOf(`#include "${name}.h"`) == -1) {
		fs.writeFileSync(`include/${allDir}/all.h`, allText.replace(/(\n#endif)/, `#include "${name}.h"\n$1`));
	}
}

function updateModule(type, name) {
	const moduleText = fs.readFileSync(`${type}/module.mk`, "utf8");
	if (moduleText.indexOf(`${type}/${name}.cpp`) == -1) {
		fs.writeFileSync(`${type}/module.mk`, moduleText.replace(/\s+$/, ` ${type}/${name}.cpp\n`));
	}
}
