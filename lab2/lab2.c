/*Это простейший модуль PAM*/

// Включаем необходимые заголовочные файлы.

#include <security/pam_modules.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

//Это определит тип нашего модуля

#define PAM_SM_AUTH
#define MAX_V 30

// Самая интересная функция.
// Именно она реализует наш неповторимый алгоритм аутентификации.
// Подобные внешние функции должны существовать во всех модулях данного класса.

PAM_EXTERN int pam_sm_authenticate(pam_handle_t * pamh, int flags
                                   ,int argc, const char **argv)
{
        unsigned int ctrl;
        int retval;
        const char *name, *p;
        char *right;
        /*special variables*/
        long x1,x2,x3,x4,y;

        time_t mytime;
        struct tm *mytm;

        /*готовимся к аутентификации*/
        mytime=time(0);
        mytm=localtime(&mytime);


        srandom(mytime);
        x1=random()%MAX_V;
        x2=random()%MAX_V;
        x3=random()%MAX_V;
        x4=random()%MAX_V;

        //завели несколько случайных величин, а заодно узнали и время.

        /* получим имя пользователя */

        // Получаем имя пользователя
        // Вся мудрость PAM в том, что приглашение "login: " появится если имя
//еще не известно,
        // иначе мы сразу получим ответ, сгенерированный предыдущими модулями.

        retval = pam_get_user(pamh, &name, "login: ");

        /*получим пароль используя диалог*/

        {
            struct pam_conv *conv;
            struct pam_message *pmsg[3],msg[3];
            struct pam_response *response;


        retval = pam_get_item( pamh, PAM_CONV, (const void **) &conv ) ;

// Сами мы не знаем как будет осущестляться диалог, это забота программы
   //(в нашем случае этим займется login). Мы
// лишь только  укажем параметры, вид приглашения и более того, можем
   //задать сразу несколько приглашений, если надо
// получить сразу несколько ответов

        pmsg[0] = &msg[0];
        msg[0].msg_style = PAM_PROMPT_ECHO_OFF;
        msg[0].msg=malloc(100);
        snprintf(msg[0].msg,60,"Second Password:%d:%d:%d:%d:",x1,x2,x3,x4);

        retval = conv->conv(1, ( const struct pam_message ** ) pmsg
                            , &response, conv->appdata_ptr);
 // Нам дали указатель на диалоговую функцию. ╢е и запускаем.
        /*просчитаем правильный ответ*/
        //y=2*x1*mytm->tm_mday+x3*mytm->tm_hour;
        y=22;
        right=malloc(100);
        snprintf(right,20,"%d",y);
 // Сравним с ответом пользователя. Ответ формируется диалоговой функцией в спе
//циальном формате.
        if (!(strcmp(right,response->resp))){
        return PAM_SUCCESS;
        }else{
        return PAM_AUTH_ERR;
        }
      }/*диалог*/
        return PAM_SUCCESS;
// Нашим результатом будет да или нет. Как прервать программу разберется основн
//ой модуль PAM.
}


/*
 * The only thing _pam_set_credentials_unix() does is initialization of
 * UNIX group IDs.
 *
 * Well, everybody but me on linux-pam is convinced that it should not
 * initialize group IDs, so I am not doing it but don't say that I haven't
 * warned you. -- AOY
 * Перевожу: ╢динственная вещь которую делает pam_set_cred это инициализация
   Идентификаторов групп... короче в данном случае это нам совершенно не нужно
 */

PAM_EXTERN int pam_sm_setcred(pam_handle_t * pamh, int flags
                              ,int argc, const char **argv)
{
        unsigned int ctrl;
        int retval;


        retval = PAM_SUCCESS;
//Чтобы никто не заметил, что мы ничего не делаем ответим, что все в порядке
        return retval;
}

// Это определение необходимо для статической линковки модулей PAM в приложения
х.
#ifdef PAM_STATIC
struct pam_module _pam_unix_auth_modstruct = {
    "pam_mypam",
    pam_sm_authenticate,
    pam_sm_setcred,
    NULL,
    NULL,
    NULL,
    NULL,
};
#endif

//sudo yum install gcc pam-devel
//gcc -shared -o pam_mypam.so -fPIC lab2.c
