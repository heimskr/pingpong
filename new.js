#!/usr/bin/env node
const fs = require("fs");

const yikes = (...a) => { console.error(...a); process.exit(1); };

const [,, ...args] = process.argv;
if (args.length != 2 || !args[1]) {
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

	fs.writeFileSync("commands/module.mk", fs.readFileSync("commands/module.mk", "utf8").replace(/\s+$/, ` commands/${name}.cpp\n`));
	allDir = "commands";
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

			virtual std::string get_command() const override;
			operator std::string() const override;
	};
}

#endif
`.substr(1);

	sourcetext = `
#include <string>

#include "messages/${name}.h"

namespace pingpong {
	std::string ${cls}::get_command() const {
		return "${name.toUpperCase()}";
	}

	${cls}::operator std::string() const {
		
	}
}
`.substr(1);

	fs.writeFileSync("messages/module.mk", fs.readFileSync("messages/module.mk", "utf8").replace(/\s+$/, ` messages/${name}.cpp\n`));
	allDir = "messages";
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

	fs.writeFileSync("lib/module.mk", fs.readFileSync("lib/module.mk", "utf8").replace(/\s+$/, ` lib/${name}.cpp\n`));
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

	fs.writeFileSync("core/module.mk", fs.readFileSync("core/module.mk", "utf8").replace(/\s+$/, ` core/${name}.cpp\n`));
	allDir = "core";
} else {
	console.error("Unknown type:", type);
	yikes(`Expected "command" | "core" | "lib" | "message"`);
}

fs.writeFileSync(sourcepath, sourcetext);
fs.writeFileSync(headerpath, headertext);

if (allDir) {
	fs.writeFileSync(`include/${allDir}/all.h`, fs.readFileSync(`include/${allDir}/all.h`, "utf8").replace(/(\n#endif)/, `#include "${name}.h"\n$1`));
}
