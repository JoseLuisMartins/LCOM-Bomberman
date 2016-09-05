#include "i8042.h"

.global _kbd_int_handler_ass

.data
makecode:
.asciz "Make Code: 0x%02X.\n"
breakcode:
.asciz "Break Code: 0x%02X.\n\n"

.lcomm twobytes, 4

.text
_kbd_int_handler_ass:
			xor %eax, %eax				// garantir eax está a 0
			in $OUT_BUF, %al			// ler o byte do out_buffer
			cmp $MSB_EXTRA_BYTE, %al	// compara com 0xE0, para verificar se tem um ou dois bytes
			jne one_byte				// nao igual significa que é one_byte
			movl $1, twobytes			// por a flag twobytes a true
			mov $0, %eax				// mover para o eax 0
			ret							// retornar
one_byte:	cmp $1, twobytes			// ve se a flag twobytes esta a true
			jne print_code				// se nao estiver, faz print de um byte
			movb $MSB_EXTRA_BYTE, %ah	// se estiver, adiciona a ah o byte mais significativo
			movl $0, twobytes			// poe twobytes a false
print_code:	push %eax					// poe o make ou breakcode na pilha
			test $BREAK_CODE_BIT, %al	// verifica se al tem o MSB a 1
			jnz break					// se não tiver, faz o print do makecode
			push $makecode				// poe a string makecode na pilha
			call _printf				// print
			jmp continue				// salta para o fim
break:		push $breakcode				// poe a string breakcode na pilha
			call _printf				// print
			cmpw  $ESC_BREAK_CODE, 4(%esp)		// vê se o breakcode é o da tecla esc (já na pilha)
			jne continue				// se não for vai para o fim
			mov $STOP, %eax 			// move para o eax STOP que vai fazer o ciclo parar
			jmp end						// salta para o fim
continue:	mov $0, %eax				// poe 0 no eax
end:		add $8, %esp
			ret
