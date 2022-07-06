
#include "emit_ir.h"

#include <string>

#include "module.h"
void EmitHir::run() {
    auto hir_output_file = _m->getModuleName();
    hir_output_file.replace(hir_output_file.end() - 2,
                            hir_output_file.end(), "hir");
    _m->HighIRprint(hir_output_file);
}
void EmitIR::run() {
    auto mir_output_file = _m->getModuleName();
    mir_output_file.replace(mir_output_file.end() - 2,
                            mir_output_file.end(), "ir");
        _m->MIRMEMprint(mir_output_file);
}
void EmitPadGraph::run() {
    _m->HIRSHOW(_m->getModuleName());
}
