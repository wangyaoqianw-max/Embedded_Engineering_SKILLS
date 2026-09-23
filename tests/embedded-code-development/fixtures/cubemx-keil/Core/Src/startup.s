        AREA    RESET, DATA, READONLY
        EXPORT  __Vectors
__Vectors
        DCD     0x20020000
        DCD     Reset_Handler

        AREA    |.text|, CODE, READONLY
        EXPORT  Reset_Handler
        IMPORT  __main
Reset_Handler PROC
        BL      __main
        B       .
        ENDP
        END
