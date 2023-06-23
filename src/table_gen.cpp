namespace beyond_all_repair
{


void gen_mips_table()
{
	printf("#include <n64/cpu.h>\n");
	printf("namespace nintendo64\n");
	printf("{\n");

	printf("const INSTR_FUNC INSTR_TABLE_DEBUG[] = \n");
	printf("{\n");

	// gen a dummy table
	for(u32 i = 0; i < INSTR_TABLE_SIZE; i++)
	{
		if(INSTR_TABLE[i].version <= MIPS3)
		{
			if(is_mem_access(INSTR_TABLE[i].fmt) || INSTR_TABLE[i].fmt == instr_type::mips_class)
			{
				printf("\t&instr_%s<true>,\n",INSTR_TABLE[i].name);
			}

			else
			{
				printf("\t&instr_%s,\n",INSTR_TABLE[i].name);
			}
		}

		else
		{
			printf("\t&instr_unknown_opcode,\n");
		}
	}

	printf("};\n");


	printf("const INSTR_FUNC INSTR_TABLE_NO_DEBUG[] = \n");
	printf("{\n");

	// gen a dummy table
	for(u32 i = 0; i < INSTR_TABLE_SIZE; i++)
	{
		if(INSTR_TABLE[i].version <= MIPS3)
		{
			if(is_mem_access(INSTR_TABLE[i].fmt) || INSTR_TABLE[i].fmt == instr_type::mips_class)
			{
				printf("\t&instr_%s<false>,\n",INSTR_TABLE[i].name);
			}

			else
			{
				printf("\t&instr_%s,\n",INSTR_TABLE[i].name);
			}
		}

		else
		{
			printf("\t&instr_unknown_opcode,\n");
		}
	}

	printf("};\n");

	printf("}\n");
}
}