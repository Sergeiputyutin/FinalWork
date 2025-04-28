#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <Windows.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    char input_a[20], input_b[20], input_c[20], input_d[20], input_e[20];
    int a1, b1, c1, d1, e1, result;
    char output[20];

    printf("y = a/b - c*d + e\n");
    printf("Введите a: ");
    scanf("%19s", input_a);
    printf("Введите b: ");
    scanf("%19s", input_b);
    printf("Введите c: ");
    scanf("%19s", input_c);
    printf("Введите d: ");
    scanf("%19s", input_d);
    printf("Введите e: ");
    scanf("%19s", input_e);

    __asm {
        // Преобразование строк в числа (исправленный способ)
        lea eax, input_a
        push eax
        call ascbin
        add esp, 4
        mov a, eax

        lea eax, input_b
        push eax
        call ascbin
        add esp, 4
        mov b, eax

        lea eax, input_c
        push eax
        call ascbin
        add esp, 4
        mov c, eax

        lea eax, input_d
        push eax
        call ascbin
        add esp, 4
        mov d, eax

        lea eax, input_e
        push eax
        call ascbin
        add esp, 4
        mov e, eax

        // Вычисление выражения
        // 1. Вычисляем a/b
        mov eax, a
        cdq
        mov ebx, b
        test ebx, ebx
        jz div_zero
        idiv ebx
        mov ecx, eax  // ecx = a/b

        // 2. Вычисляем c*d
        mov eax, c
        imul d        // eax = c*d

        // 3. Вычисляем a/b - c*d + e
        sub ecx, eax  // ecx = (a/b - c*d)
        add ecx, e    // ecx = (a/b - c*d) + e
        mov result, ecx
        jmp calc_done

        div_zero :
        mov result, 0

            calc_done :
            // Преобразуем результат в строку
            lea eax, output
            push eax
            push result
            call binasc
            add esp, 8

            jmp end_program

            // Процедура преобразования строки в число (исправленная)
            ascbin :
        push ebp
            mov ebp, esp
            push ebx
            push esi

            mov esi, [ebp + 8]  // получаем адрес строки из стека
            xor eax, eax      // обнуляем результат
            xor ebx, ebx // флаг знака (0 = положительное)
            xor ecx, ecx

            // Проверка знака
            mov cl, [esi]
            cmp cl, '+'
                je next_char
                cmp cl, '-'
                jne digit_loop
                mov bl, 1         // установим флаг отрицательного числа
                next_char:
            inc esi
                digit_loop :
            mov cl, [esi]
                cmp cl, 0         // конец строки?
                    je end_convert
                    cmp cl, '0'
                    jb invalid
                    cmp cl, '9'
                    ja invalid

                    // Преобразование цифры
                    imul eax, 10
                    sub cl, '0'

                    add eax, ecx
                    inc esi
                    jmp digit_loop
                    invalid :
                xor eax, eax      // при ошибке возвращаем 0
                    end_convert :
                test bl, bl
                    jz positive
                    neg eax
                    positive :
                pop esi
                    pop ebx
                    pop ebp
                    ret

                    // Процедура преобразования числа в строку
                    binasc :
                push ebp
                    mov ebp, esp
                    push ebx
                    push esi
                    push edi

                    mov eax, [ebp + 8]   // число
                    mov edi, [ebp + 12]  // буфер для строки
                    xor ebx, ebx       // флаг знака

                    // Проверка знака
                    test eax, eax
                    jns positive_num
                    mov bl, 1          // число отрицательное
                    neg eax
                    positive_num :
                // Преобразуем цифры
                mov ecx, 10
                    xor esi, esi       // счетчик цифр
                    convert_loop :
                xor edx, edx
                    div ecx            // eax = eax/10, edx = остаток
                    add dl, '0'
                    push edx
                    inc esi
                    test eax, eax
                    jnz convert_loop

                    // Добавляем знак минус если нужно
                    test bl, bl
                    jz no_sign
                    mov byte ptr[edi], '-'
                    inc edi
                    no_sign :
                // Извлекаем цифры из стека
            pop_loop:
                pop eax
                    mov[edi], al
                    inc edi
                    dec esi
                    jnz pop_loop

                    // Завершаем строку нулем
                    mov byte ptr[edi], 0

                    pop edi
                    pop esi
                    pop ebx
                    pop ebp
                    ret

                    end_program :
    }
    int M[20], i, j = -10;
    for (i = 0; i <= 10; i++) {
        M[i] += j;
        j += 1;
    }
    printf("Результат: y = %d\n", result);
    return 0;
}
