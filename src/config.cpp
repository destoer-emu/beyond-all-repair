#include <beyond_all_repair.h>

namespace beyond_all_repair
{
	Config parse_config(const char* flags)
	{
		Config config;

		while(*flags)
		{
			auto c = *flags;

			switch(c)
			{
				case 'd': config.disassemble = true; break;
				case 'g': config.gen = true; break;
				case 'm': config.gen_blank = true; break;
				case 'a': config.print_addr = true; break;
				case 'o': config.print_opcodes = true; break;
				case 'e': config.print_external = true; break;
				case 'l': config.linear = true; break;
				default: break;
			}

			flags++;
		}

		return config;
	}
}