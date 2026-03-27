#include "protocol-task.h"
#include "stdio.h"
#include "string.h"

#include "pico/stdlib.h"

static api_t* api = {0};
static int commands_count = 0;

void protocol_task_init(api_t* device_api){
    api = device_api;
    commands_count = 0;
        
    while (api[commands_count].command_name != NULL) {
            commands_count++;
        }
    printf(" Найденно '%d' команд\n", commands_count); 

}

void protocol_task_handle(char* command_string)

{
//command_string[strcspn(command_string, "\r\n")] = '\0';
//	Добавляем в обработчик проверку на то, что `command_string` не равно `NULL`.
//	Если `command_string` равно `NULL`, то выйти из обработчика: строка команды
//	еще не получена;
if (!command_string)
{
return;
}


// логика обработки полученной строки. Делим ее на команду и аргументы:
 const char* command_name = command_string;
 const char* command_args = NULL;

 char* space_symbol = strchr(command_string, ' ');

if (space_symbol)
 {
 *space_symbol = '\0';
 command_args = space_symbol + 1;
 }
 else
 {
command_args = "";
 }

  
// Добавляем вывод найденных имени команды и ее аргументов;
printf("received command: '%s'\n", command_name);
printf("received args: '%s'\n", command_args);

  
// в цикле проходим по массиву команд `api` и ищем совпадение имени команды;

 for (int i = 0; i < commands_count; i++)
 {

    if (strcmp(api[i].command_name,command_name)==0)
        {
            api[i].command_callback(command_args);
            return;
        }
}
 printf("Error: Unknown command '%s'\n", command_name);
 return;
}

void help_handle(void)
{
    for (int i = 0; api[i].command_name != NULL; i++) 
    {
        printf("'%s' - '%s'\n", api[i].command_name, api[i].command_help);
    }
}