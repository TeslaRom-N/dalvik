/*
 * In the C mterp stubs, "goto" is a function call followed immediately
 * by a return.
 */

#define GOTO_TARGET_DECL(_target, ...)

#define GOTO_TARGET(_target, ...) _target:

#define GOTO_TARGET_END

/* ugh */
#define STUB_HACK(x)
#define JIT_STUB_HACK(x)

/*
 * Instruction framing.  For a switch-oriented implementation this is
 * case/break, for a threaded implementation it's a goto label and an
 * instruction fetch/computed goto.
 *
 * Assumes the existence of "const u2* pc" and (for threaded operation)
 * "u2 inst".
 */
# define H(_op)             &&op_##_op
# define HANDLE_OPCODE(_op) op_##_op:
# define FINISH(_offset) {                                                  \
        ADJUST_PC(_offset);                                                 \
        inst = FETCH(0);                                                    \
        if (self->interpBreak.ctl.subMode) {                                \
            dvmCheckBefore(pc, fp, self);                                   \
        }                                                                   \
        goto *handlerTable[INST_INST(inst)];                                \
    }
# define FINISH_BKPT(_opcode) {                                             \
        goto *handlerTable[_opcode];                                        \
    }
# define DISPATCH_EXTENDED(_opcode) {                                       \
        goto *handlerTable[0x100 + _opcode];                                \
    }

#define OP_END

/*
 * The "goto" targets just turn into goto statements.  The "arguments" are
 * passed through local variables.
 */

#define GOTO_exceptionThrown() goto exceptionThrown;

#define GOTO_returnFromMethod() goto returnFromMethod;

#define GOTO_invoke(_target, _methodCallRange, _jumboFormat)                \
    do {                                                                    \
        methodCallRange = _methodCallRange;                                 \
        jumboFormat = _jumboFormat;                                         \
        goto _target;                                                       \
    } while(false)

/* for this, the "args" are already in the locals */
#define GOTO_invokeMethod(_methodCallRange, _methodToCall, _vsrc1, _vdst) goto invokeMethod;

#define GOTO_bail() goto bail;

/*
 * Periodically check for thread suspension.
 *
 * While we're at it, see if a debugger has attached or the profiler has
 * started.  If so, switch to a different "goto" table.
 */
#define PERIODIC_CHECKS(_pcadj) {                              \
        if (dvmCheckSuspendQuick(self)) {                                   \
            EXPORT_PC();  /* need for precise GC */                         \
            dvmCheckSuspendPending(self);                                   \
        }                                                                   \
    }
