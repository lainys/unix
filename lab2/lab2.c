/*Это простейший модуль PAM*/

// Включаем необходимые заголовочные файлы.

#include <security/pam_modules.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define PAM_SM_AUTH


PAM_EXTERN int pam_sm_authenticate(pam_handle_t * pamh, int flags
                                   ,int argc, const char **argv)
{
        unsigned int ctrl;
        int retval;
        const char *name, *p;
        char *right;

        long y,x;

        time_t mytime;
        struct tm *mytm;

        mytime=time(0);
        mytm=localtime(&mytime);

        srandom(mytime);
        x=random()%100;

        retval = pam_get_user(pamh, &name, "login: ");

        
        struct pam_conv *conv;
        struct pam_message *pmsg[3],msg[3];
        struct pam_response *response;


        retval = pam_get_item( pamh, PAM_CONV, (const void **) &conv );

        pmsg[0] = &msg[0];
        msg[0].msg_style = PAM_PROMPT_ECHO_OFF;
        msg[0].msg=malloc(100);
        snprintf(msg[0].msg,60,"Password: (%d:%d,%d)",mytm->tm_hour,mytm->tm_min,x);

        retval = conv->conv(1, ( const struct pam_message ** ) pmsg
                            , &response, conv->appdata_ptr);

        int hour = mytm->tm_hour;
        int min = mytm->tm_min;
        y = hour - min + x;
        right = malloc(100);
        snprintf(right,20,"%d",y);

        if (!(strcmp(right,response->resp))){
            return PAM_SUCCESS;
        }else{
            return PAM_AUTH_ERR;
        }
        return PAM_SUCCESS;
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
        return retval;
}

// Это определение необходимо для статической линковки модулей PAM в приложения
//х.
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
