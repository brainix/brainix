/*----------------------------------------------------------------------------*\
 |	main.c								      |
 |									      |
 |	Copyright � 2002-2007, Team Brainix, original authors.		      |
 |		All rights reserved.					      |
\*----------------------------------------------------------------------------*/

/*
 | This program is free software: you can redistribute it and/or modify it under
 | the terms of the GNU General Public License as published by the Free Software
 | Foundation, either version 3 of the License, or (at your option) any later
 | version.
 |
 | This program is distributed in the hope that it will be useful, but WITHOUT
 | ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 | FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 | details.
 |
 | You should have received a copy of the GNU General Public License along with
 | this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define BRAINIX
#include <kernel/kernel.h>

/* Function prototypes: */
void main(unsigned long magic, unsigned long multiboot_info_addr);
void kernel(void);
void shutdown(msg_t *msg);
void print_init(bool init, char *s);
void print_done(void);

/*----------------------------------------------------------------------------*\
 |				     main()				      |
\*----------------------------------------------------------------------------*/
void main(unsigned long magic, unsigned long multiboot_info_addr)
{

/* After the machine boots (and GRUB loads Brainix), this is the first C
 * function to run.  Tip the first domino... */

	multiboot_info_t *multiboot_info = (multiboot_info_t *)
		multiboot_info_addr;

	set_attr(SOLID, NORMAL_BG, NORMAL_FG);
	clear();
	set_attr(SOLID, SEVERE_BG, SEVERE_FG);
	print_version();
	set_attr(SOLID, NORMAL_BG, NORMAL_FG);
	printf("\n");

	print_init(true, "hardware");
	seg_init();
	intr_init();
	print_done();

	print_init(true, "kernel");
	paging_init(multiboot_info->mem_upper);
	proc_init();
	msg_init();
	irq_init();
	proc_create((unsigned long) kernel, KERNEL);
	print_done();

	print_init(true, "timer");
	proc_create((unsigned long) timer_main, KERNEL);
	print_done();

	print_init(true, "file system");
	proc_create((unsigned long) fs_main, KERNEL);
	print_done();

	print_init(true, "drivers");
	proc_create((unsigned long) mem_main, KERNEL);
	proc_create((unsigned long) fdc_main, KERNEL);
	proc_create((unsigned long) tty_main, KERNEL);
	print_done();

	printf("\n");
	proc_sched();
	while (true)
		proc_sched();

//		halt();
}

/*----------------------------------------------------------------------------*\
 |				    kernel()				      |
\*----------------------------------------------------------------------------*/
void kernel(void)
{
	msg_t *msg;

	while (true)
		switch ((msg = msg_receive(ANYONE))->op)
		{
			case SYS_FORK:
				proc_fork(msg);
				msg_reply(msg);
				continue;
			case SYS_EXECVE:
				proc_exec(msg);
				msg_free(msg);
				continue;
			case SYS_EXIT:
				proc_exit(msg);
				msg_free(msg);
				continue;
			case REGISTER:
				irq_register(msg);
				msg_reply(msg);
				continue;
			case SHUTDOWN:
				shutdown(msg);
				msg_free(msg);
				continue;
			default:
				panic("kernel", "unexpected message");
				msg_free(msg);
				continue;
		}
}

/*----------------------------------------------------------------------------*\
 |				   shutdown()				      |
\*----------------------------------------------------------------------------*/
void shutdown(msg_t *msg)
{
	printf("\n");

	print_init(false, "user processes");
	print_done();

	print_init(false, "file system");
	print_done();

	print_init(false, "timer");
	msg_send(msg_alloc(TMR_PID, SHUTDOWN));
	msg_free(msg_receive(TMR_PID));
	print_done();

	print_init(false, "hardware");
	if (msg->args.brnx_shutdown.reboot)
		kbd_reboot();
	print_done();

	disable_intrs();
	halt();
}

/*----------------------------------------------------------------------------*\
 |				  print_init()				      |
\*----------------------------------------------------------------------------*/
void print_init(bool init, char *s)
{
	unsigned char j;

	set_attr(SOLID, UNIMPORTANT_BG, UNIMPORTANT_FG);
	if (!init)
		printf("de");
	printf("init");
	set_attr(SOLID, NORMAL_BG, NORMAL_FG);
	printf(" %s ", s);
	for (j = strlen(s); j < strlen("file system"); j++)
		printf(" ");
	set_attr(SOLID, UNIMPORTANT_BG, UNIMPORTANT_FG);
	printf("...");
	set_attr(SOLID, NORMAL_BG, NORMAL_FG);
	printf(" ");
	print_emoticon(THINK);
}

/*----------------------------------------------------------------------------*\
 |				  print_done()				      |
\*----------------------------------------------------------------------------*/
void print_done(void)
{
	print_emoticon(ERASE);
	print_emoticon(SMILE);
	printf("\n");
}
