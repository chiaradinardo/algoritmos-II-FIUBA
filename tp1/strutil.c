#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "strutil.h"

char *substr(const char *str, size_t n){
    char* subcadena = calloc(n + 1, sizeof(char));

    if(!subcadena){
        return NULL;
    }

    size_t pos = strlen(str);

    for(size_t i = 0; i < n && i < pos; i++){
        subcadena[i] = str[i];
    }

    return subcadena;
}

char **split(const char *str, char sep){
    size_t largo = 1;
    for(size_t i = 0; i < strlen(str); i++){
        if(str[i] == sep){
            largo++;
        }
    }

    char** strv = calloc(largo + 1, sizeof(char*));
    if(!strv){
        return NULL;
    }

    size_t pos = 0;
    size_t j = 0;

    for(size_t i = 0; pos < largo; i++){
        if(str[i] == sep || str[i] == '\0'){
            strv[pos] = strndup(&str[i - j], j);
            pos++;
            j = 0;
            continue;
        }
        j++;
    }

    return strv;

}

char *join(char **strv, char sep){
    size_t n = 0;
    for(size_t i = 0; strv[i] != NULL; i++){
        n += strlen(strv[i]) + 1;
    }

    char* resultado = malloc((n + 1) * sizeof(char));
    if(!resultado){
        return NULL;
    }

    size_t pos = 0;

    for(size_t i = 0; strv[i] != NULL; i++){
        for(size_t j = 0; strv[i][j] != '\0'; j++){
            resultado[pos] = strv[i][j];
            pos++;
        }

        if(pos == n - 1){
            break;
        }
        if(sep != '\0'){
            resultado[pos] = sep;
            pos++;
        }
    }

    resultado[pos] = '\0';
    
    return resultado;
}

void free_strv(char *strv[]){
    for(size_t i = 0; strv[i] != NULL; i++){
        free(strv[i]);
    }
    free(strv);
}
