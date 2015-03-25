#ifndef STATIC_ARRAY_LIST_H_
#define STATIC_ARRAY_LIST_H_
////////// VERSION 1 - POOL SIMPLE: no permite devolver nodos al pool //////////


/**
 * Escribe un type def para nodos que contendran elementos del tipo especificado
 * -no admite tipos con espacios-
 */
#define STATIC_AL_GENERATE_NODE_DEF( elementType ) struct sALNode_ ## elementType { elementType value; struct sALNode_ ## elementType * next; }; typedef struct sALNode_ ## elementType sALNode_ ## elementType;

/**
 * Inicializa un pool de nodos basado en un arreglo
 */
#define STATIC_AL_INITIALIZE_POOL( arrayName, elementType, numberOfNodes )\
    sALNode_ ## elementType arrayName[numberOfNodes];\
    int arrayName ## MaxSize = numberOfNodes;\
    int arrayName ## NextNodeIndexAvailable = 0; 

/**
 * Crea la funcion que obtiene un nodo desde el pool de nodos especificado.
 */
#define STATIC_AL_CREATE_GIVE_NODE_FUNCTION( arrayName, elementType )\
    sALNode_ ## elementType* giveNodeFrom ## arrayName(void){\
        if( arrayName ## NextNodeIndexAvailable >= arrayName ## MaxSize ){\
            printf( "There are no more nodes to give; unrecoverable programming error detected by STATIC_ARRAY_LISTs POOL. Continuing, but program will fail." );\
        }\
        return arrayName + (arrayName ## NextNodeIndexAvailable ++) ;\
    }

#define STATIC_AL_INITIALIZE_LINEAR( arrayName, arrayLength) \
    int arrayName ## tmp = 0 ;\
    for( ; arrayName ## tmp < arrayLength-1; arrayName ## tmp++){\
        arrayName[arrayName ## tmp].next = &arrayName[arrayName ## tmp+1];\
    }\
    arrayName ## tmp = 0;\
    for(arrayName ## tmp = 0; arrayName ## tmp < arrayLength; arrayName ## tmp++){\
        arrayName[arrayName ## tmp].value = 0;\
    }

 

#endif /*STATIC_ARRAY_LIST_H_*/
