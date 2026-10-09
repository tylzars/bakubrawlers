/* shared declarations for the CRI slice 0x0203e954-0x0204046c */
typedef struct CriObj {
    short used;                       /* 0x00 */
    short s2;
    int state;                        /* 0x04 */
    int h;                            /* 0x08 */
    signed char c;                    /* 0x0c */
    signed char d, e, f;              /* 0x0d.. */
    int i10, i14, i18;
    short s1c;
    short pad1e;
    int i20;
    short s24, s26;
    int i28, i2c, i30, i34;
    int i38, i3c, i40, i44, i48, i4c;
    int i50, i54, i58;
    short *i5c;                       /* 0x5c */
    int i60, i64;
    int i68, i6c, i70;
    int i74;
    void (*cb78)(int, int *, int *, int *);
    int i7c;
    void (*cb80)(int, int, int);
    int i84;
    int i88, i8c, i90, i94;
    short s98, s9a;
    short pad9c[2];
    short sa0, sa2, sa4, sa6, sa8, saa;
    int iac;
    int ib0;
    int ib4;
    int pad_b8[2];
    int ic0;
    signed char bc4[16];
    short sd4, sd6, sd8;
    short padda;
    int idc, ie0;
    void (*cbe4)(int, int, int);
    int ie8;
} CriObj;

