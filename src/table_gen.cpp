namespace beyond_all_repair
{


void print_delmit(u32 i)
{
	if((i & 0b111) == 0b000)
	{
		putchar('\n');
	}
}


void gen_blank()
{
	for(u32 i = 0; i <= 0b111'111; i++)
	{
		print_delmit(i);
		//print("{{\"unknown_opcode\",instr_type::unk,nullptr,&unknown_disass_cop0,MIPS1}}, //0b{:03b}'{:03b}\n",(i >> 2) & 0b111,(i >> 0) & 0b11);
		print("{{\"unknown_opcode\",instr_type::unk,nullptr,&unknown_disass_cop0,MIPS1}}, //0b{:03b}'{:03b}\n",(i >> 3) & 0b111,(i >> 0) & 0b111);
	}	
}

void gen_mips_table(int argc, char* argv[])
{
	if(argc != 4)
	{
		printf("usage: <bar> -g <mips-version> <output>\n");
		return;
	}


	FILE* fp = fopen(argv[3],"w");

	if(!fp)
	{
		printf("could not create table output file: %s\n",argv[3]);
		return;
	}

	const u32 version = atoi(argv[2]);

	printf("generating instruction table version: %d -> %s\n",version,argv[3]);

	fprintf(fp,"const INSTR_FUNC INSTR_TABLE_DEBUG[] = \n");
	fprintf(fp,"{\n");

	// gen a dummy table
	for(u32 i = 0; i < INSTR_TABLE_SIZE; i++)
	{
		if(INSTR_TABLE[i].version <= version)
		{
			if(is_mem_access(INSTR_TABLE[i].fmt) || INSTR_TABLE[i].fmt == instr_type::mips_class)
			{
				fprintf(fp,"\t&instr_%s<true>,\n",INSTR_TABLE[i].name);
			}

			else
			{
				// floating point func replace '.' with '_' for name
				if(strchr(INSTR_TABLE[i].name, '.'))
				{
					std::string name = INSTR_TABLE[i].name;

					for(auto& c : name)
					{
						if(c == '.')
						{
							c = '_';
						}
					}

					fprintf(fp,"\t&instr_%s,\n",name.c_str());
				}

				else
				{
					fprintf(fp,"\t&instr_%s,\n",INSTR_TABLE[i].name);
				}
			}
		}

		else
		{
			fprintf(fp,"\t&instr_unknown_opcode,\n");
		}
	}

	fprintf(fp,"};\n");


	fprintf(fp,"const INSTR_FUNC INSTR_TABLE_NO_DEBUG[] = \n");
	fprintf(fp,"{\n");

	// gen a dummy table
	for(u32 i = 0; i < INSTR_TABLE_SIZE; i++)
	{
		if(INSTR_TABLE[i].version <= version)
		{
			if(is_mem_access(INSTR_TABLE[i].fmt) || INSTR_TABLE[i].fmt == instr_type::mips_class)
			{
				fprintf(fp,"\t&instr_%s<false>,\n",INSTR_TABLE[i].name);
			}

			else
			{
				// floating point func replace '.' with '_' for name
				if(strchr(INSTR_TABLE[i].name, '.'))
				{
					std::string name = INSTR_TABLE[i].name;

					for(auto& c : name)
					{
						if(c == '.')
						{
							c = '_';
						}
					}

					fprintf(fp,"\t&instr_%s,\n",name.c_str());
				}

				else
				{
					fprintf(fp,"\t&instr_%s,\n",INSTR_TABLE[i].name);
				}
			}
		}

		else
		{
			fprintf(fp,"\t&instr_unknown_opcode,\n");
		}
	}

	fprintf(fp,"};\n");
}
}