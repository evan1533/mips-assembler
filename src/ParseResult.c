#include "ParseResult.h"
#include <stdlib.h>
#include <string.h>

/**  Frees the dynamic content of a ParseResult object.
 *
 *   Pre:  pPR points to a proper ParseResult object.
 *   Post: All of the dynamically-allocated arrays in *pPR have been
 *         deallocated; pointers are NULL, static fields are reset to
 *         default values.
 *
 *   Comments:
 *     -  The function has no information about whether *pPR has been
 *        allocated dynamically, so it cannot risk attempting to
 *        deallocate *pPR.
 *     -  The function is intended to provide the user with a simple
 *        way to free memory; the user may or may not reuse *pPR.  So,
 *        the function does set the pointers in *pPR to NULL.
 */
void clearResult(ParseResult* const pPR)
{
   if(strcmp(pPR->Opcode, "000000") == 0)
   {
      free(pPR->rdName);
       free(pPR->RD);
        free(pPR->Funct);
   }
   else
   {
      free(pPR->IMM);
   }

   free(pPR->ASMInstruction);
    free(pPR->Mnemonic);
    free(pPR->rtName);
   free(pPR->RT);
   free(pPR->RS);
    free(pPR->rsName);
    free(pPR->Opcode);

   pPR->ASMInstruction = NULL;
   pPR->Mnemonic = NULL;
   pPR->rdName = NULL;
   pPR->rsName = NULL;
   pPR->rtName = NULL;
   pPR->Imm = 0;
   pPR->rd = 255;
   pPR->rs = 255;
   pPR->rt = 255;
   pPR->Opcode = NULL;
   pPR->Funct = NULL;
   pPR->RD = NULL;
   pPR->RS = NULL;
   pPR->RT = NULL;
   pPR->IMM = NULL;
}

/**  Prints the contents of a ParseResult object.
 *
 *   Pre:  Log is open on an output file.
 *         pPR points to a proper ParseResult object.
 */
void printResult(FILE* Log, const ParseResult* const pPR) {

      fprintf(Log, "%s\n", pPR->ASMInstruction);
      fprintf(Log, "   %s   %s\n", pPR->Opcode, pPR->Mnemonic);
      fprintf(Log, "   %2"PRIu8"   %s", pPR->rd, pPR->rdName);
      if ( pPR->RD != NULL ) {
            fprintf(Log, "   %s", pPR->RD);
        }
        fprintf(Log, "\n");
      fprintf(Log, "   %2"PRIu8"   %s", pPR->rs, pPR->rsName);
      if ( pPR->RS != NULL ) {
            fprintf(Log, "   %s", pPR->RS);
        }
        fprintf(Log, "\n");
      fprintf(Log, "   %2"PRIu8"   %s", pPR->rt, pPR->rtName);
      if ( pPR->RT != NULL ) {
            fprintf(Log, "   %s", pPR->RT);
        }
        fprintf(Log, "\n");
      fprintf(Log, "   %s\n", pPR->Funct);
      fprintf(Log, "   %"PRId16"\n", pPR->Imm);
      if ( pPR->IMM != NULL ) {
            fprintf(Log, "   %s", pPR->IMM);
        }
        fprintf(Log, "\n");
        fprintf(Log, "\n");
}
