#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <errno.h>
#include <math.h>

const double M_PI = 3.14;

#define PERIOD_CNT 20

int main (int argc , char* argv[])
{
        FILE *file;
        int i, c, f = 10, j, d = 0;
        unsigned int *buf;
        double ampl;
        short ipart, qpart;

        f = atoi(argv[2]);
        ampl = atof(argv[1]);

        buf = (unsigned int*)malloc(f * PERIOD_CNT * (d ? 8 : 4));

        if (ampl > 1.0)
                ampl = 1.0;
        else if (ampl < 0.0)
                ampl = 0.0;

        /* AD9361 12-bit MSB aligned [(2^(12-1) - 1) * 16]
         * AD9122 16-bit             [2^(16-1) - 1]
         */

        ampl = ampl * 32767;

        printf("32-bit Word: I : Q\n");

        for (i = 0, j = 0; i < (f) * PERIOD_CNT; i++) {
                ipart = ampl * sin(2 * M_PI * (double)i / (double)(f));
               qpart = ampl * cos(2 * M_PI * (double)i / (double)(f));
			
			//	ipart = ampl * sin(2 * M_PI * (double)i * 32000. / 2000000.);
			//	qpart = ampl * cos(2 * M_PI * (double)i * 32000. / 2000000.);

                printf("0x%.8X : %d : %d\n", (ipart << 16) | (qpart & 0xFFFF),  ipart, qpart);

                buf[j++] = (ipart << 16) | (qpart & 0xFFFF);

                if (d) /* Second Channel */
                        buf[j++] = (ipart << 16) | (qpart & 0xFFFF);

        }

        file = fopen(argv[3], "w");
        if (file == NULL) {
                free(buf);
                exit(EXIT_FAILURE);
        }

        fwrite(buf, (d ? 8 : 4), f * PERIOD_CNT, file);
        fclose(file);
        free(buf);

        exit(EXIT_SUCCESS);
}
