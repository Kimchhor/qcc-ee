; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt -S -aarch64-sve-intrinsic-opts -mtriple=aarch64-linux-gnu -mattr=+sve < %s | FileCheck %s

declare <vscale x 16 x i1> @llvm.aarch64.sve.ptrue.nxv16i1(i32 immarg)
declare <vscale x 2 x i1> @llvm.aarch64.sve.ptrue.nxv2i1(i32 immarg)
declare <vscale x 4 x i1> @llvm.aarch64.sve.ptrue.nxv4i1(i32 immarg)
declare <vscale x 8 x i1> @llvm.aarch64.sve.ptrue.nxv8i1(i32 immarg)

declare <vscale x 16 x i32> @llvm.aarch64.sve.ld1.nxv16i32(<vscale x 16 x i1>, i32*)
declare <vscale x 2 x i32> @llvm.aarch64.sve.ld1.nxv2i32(<vscale x 2 x i1>, i32*)
declare <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1>, i32*)
declare <vscale x 8 x i16> @llvm.aarch64.sve.ld1.nxv8i16(<vscale x 8 x i1>, i16*)
declare <vscale x 8 x i32> @llvm.aarch64.sve.ld1.nxv8i32(<vscale x 8 x i1>, i32*)

declare <vscale x 16 x i1> @llvm.aarch64.sve.convert.to.svbool.nxv4i1(<vscale x 4 x i1>)
declare <vscale x 8 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv4i1(<vscale x 16 x i1>)

; Two calls to the SVE ptrue intrinsic. %1 is redundant, and can be expressed as an SVE reinterpret of %3 via
; convert.{to,from}.svbool.
define <vscale x 8 x i32> @coalesce_test_basic(i32* %addr) {
; CHECK-LABEL: @coalesce_test_basic(
; CHECK-NEXT:    [[TMP1:%.*]] = call <vscale x 8 x i1> @llvm.aarch64.sve.ptrue.nxv8i1(i32 31)
; CHECK-NEXT:    [[TMP2:%.*]] = call <vscale x 16 x i1> @llvm.aarch64.sve.convert.to.svbool.nxv8i1(<vscale x 8 x i1> [[TMP1]])
; CHECK-NEXT:    [[TMP3:%.*]] = call <vscale x 4 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv4i1(<vscale x 16 x i1> [[TMP2]])
; CHECK-NEXT:    [[TMP4:%.*]] = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> [[TMP3]], i32* [[ADDR:%.*]])
; CHECK-NEXT:    [[TMP5:%.*]] = call <vscale x 8 x i32> @llvm.aarch64.sve.ld1.nxv8i32(<vscale x 8 x i1> [[TMP1]], i32* [[ADDR]])
; CHECK-NEXT:    ret <vscale x 8 x i32> [[TMP5]]
;
  %1 = call <vscale x 4 x i1> @llvm.aarch64.sve.ptrue.nxv4i1(i32 31)
  %2 = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> %1, i32* %addr)
  %3 = call <vscale x 8 x i1> @llvm.aarch64.sve.ptrue.nxv8i1(i32 31)
  %4 = call <vscale x 8 x i32> @llvm.aarch64.sve.ld1.nxv8i32(<vscale x 8 x i1> %3, i32* %addr)
  ret <vscale x 8 x i32> %4
}

; Two calls to the SVE ptrue intrinsic with the SV_POW2 pattern. This should reduce to the same output as
; coalesce_test_basic.
define <vscale x 8 x i32> @coalesce_test_pow2(i32* %addr) {
; CHECK-LABEL: @coalesce_test_pow2(
; CHECK-NEXT:    [[TMP1:%.*]] = call <vscale x 8 x i1> @llvm.aarch64.sve.ptrue.nxv8i1(i32 0)
; CHECK-NEXT:    [[TMP2:%.*]] = call <vscale x 16 x i1> @llvm.aarch64.sve.convert.to.svbool.nxv8i1(<vscale x 8 x i1> [[TMP1]])
; CHECK-NEXT:    [[TMP3:%.*]] = call <vscale x 4 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv4i1(<vscale x 16 x i1> [[TMP2]])
; CHECK-NEXT:    [[TMP4:%.*]] = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> [[TMP3]], i32* [[ADDR:%.*]])
; CHECK-NEXT:    [[TMP5:%.*]] = call <vscale x 8 x i32> @llvm.aarch64.sve.ld1.nxv8i32(<vscale x 8 x i1> [[TMP1]], i32* [[ADDR]])
; CHECK-NEXT:    ret <vscale x 8 x i32> [[TMP5]]
;
  %1 = call <vscale x 4 x i1> @llvm.aarch64.sve.ptrue.nxv4i1(i32 0)
  %2 = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> %1, i32* %addr)
  %3 = call <vscale x 8 x i1> @llvm.aarch64.sve.ptrue.nxv8i1(i32 0)
  %4 = call <vscale x 8 x i32> @llvm.aarch64.sve.ld1.nxv8i32(<vscale x 8 x i1> %3, i32* %addr)
  ret <vscale x 8 x i32> %4
}

; Four calls to the SVE ptrue intrinsic; two with the SV_ALL patterns, and two with the SV_POW2 pattern. The
; two SV_ALL ptrue intrinsics should be coalesced, and the two SV_POW2 intrinsics should be colaesced.
define <vscale x 8 x i32> @coalesce_test_all_and_pow2(i32* %addr) {
; CHECK-LABEL: @coalesce_test_all_and_pow2(
; CHECK-NEXT:    [[TMP1:%.*]] = call <vscale x 8 x i1> @llvm.aarch64.sve.ptrue.nxv8i1(i32 0)
; CHECK-NEXT:    [[TMP2:%.*]] = call <vscale x 16 x i1> @llvm.aarch64.sve.convert.to.svbool.nxv8i1(<vscale x 8 x i1> [[TMP1]])
; CHECK-NEXT:    [[TMP3:%.*]] = call <vscale x 4 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv4i1(<vscale x 16 x i1> [[TMP2]])
; CHECK-NEXT:    [[TMP4:%.*]] = call <vscale x 8 x i1> @llvm.aarch64.sve.ptrue.nxv8i1(i32 31)
; CHECK-NEXT:    [[TMP5:%.*]] = call <vscale x 16 x i1> @llvm.aarch64.sve.convert.to.svbool.nxv8i1(<vscale x 8 x i1> [[TMP4]])
; CHECK-NEXT:    [[TMP6:%.*]] = call <vscale x 4 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv4i1(<vscale x 16 x i1> [[TMP5]])
; CHECK-NEXT:    [[TMP7:%.*]] = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> [[TMP3]], i32* [[ADDR:%.*]])
; CHECK-NEXT:    [[TMP8:%.*]] = call <vscale x 8 x i32> @llvm.aarch64.sve.ld1.nxv8i32(<vscale x 8 x i1> [[TMP1]], i32* [[ADDR]])
; CHECK-NEXT:    [[TMP9:%.*]] = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> [[TMP6]], i32* [[ADDR]])
; CHECK-NEXT:    [[TMP10:%.*]] = call <vscale x 8 x i32> @llvm.aarch64.sve.ld1.nxv8i32(<vscale x 8 x i1> [[TMP4]], i32* [[ADDR]])
; CHECK-NEXT:    ret <vscale x 8 x i32> [[TMP10]]
;
  %1 = call <vscale x 4 x i1> @llvm.aarch64.sve.ptrue.nxv4i1(i32 0)
  %2 = call <vscale x 8 x i1> @llvm.aarch64.sve.ptrue.nxv8i1(i32 0)
  %3 = call <vscale x 4 x i1> @llvm.aarch64.sve.ptrue.nxv4i1(i32 31)
  %4 = call <vscale x 8 x i1> @llvm.aarch64.sve.ptrue.nxv8i1(i32 31)

  %5 = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> %1, i32* %addr)
  %6 = call <vscale x 8 x i32> @llvm.aarch64.sve.ld1.nxv8i32(<vscale x 8 x i1> %2, i32* %addr)
  %7 = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> %3, i32* %addr)
  %8 = call <vscale x 8 x i32> @llvm.aarch64.sve.ld1.nxv8i32(<vscale x 8 x i1> %4, i32* %addr)
  ret <vscale x 8 x i32> %8
}


; Two calls to the SVE ptrue intrinsic: one with the SV_ALL pattern, another with the SV_POW2 pattern. The
; patterns are incompatible, so they should not be coalesced.
define <vscale x 8 x i32> @coalesce_test_pattern_mismatch2(i32* %addr) {
; CHECK-LABEL: @coalesce_test_pattern_mismatch2(
; CHECK-NEXT:    [[TMP1:%.*]] = call <vscale x 4 x i1> @llvm.aarch64.sve.ptrue.nxv4i1(i32 0)
; CHECK-NEXT:    [[TMP2:%.*]] = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> [[TMP1]], i32* [[ADDR:%.*]])
; CHECK-NEXT:    [[TMP3:%.*]] = call <vscale x 8 x i1> @llvm.aarch64.sve.ptrue.nxv8i1(i32 31)
; CHECK-NEXT:    [[TMP4:%.*]] = call <vscale x 8 x i32> @llvm.aarch64.sve.ld1.nxv8i32(<vscale x 8 x i1> [[TMP3]], i32* [[ADDR]])
; CHECK-NEXT:    ret <vscale x 8 x i32> [[TMP4]]
;
  %1 = call <vscale x 4 x i1> @llvm.aarch64.sve.ptrue.nxv4i1(i32 0)
  %2 = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> %1, i32* %addr)
  %3 = call <vscale x 8 x i1> @llvm.aarch64.sve.ptrue.nxv8i1(i32 31)
  %4 = call <vscale x 8 x i32> @llvm.aarch64.sve.ld1.nxv8i32(<vscale x 8 x i1> %3, i32* %addr)
  ret <vscale x 8 x i32> %4
}

; Two calls to the SVE ptrue intrinsic with the SV_VL1 pattern. This pattern is not currently recognised, so
; nothing should be done here.
define <vscale x 8 x i32> @coalesce_test_bad_pattern(i32* %addr) {
; CHECK-LABEL: @coalesce_test_bad_pattern(
; CHECK-NEXT:    [[TMP1:%.*]] = call <vscale x 4 x i1> @llvm.aarch64.sve.ptrue.nxv4i1(i32 1)
; CHECK-NEXT:    [[TMP2:%.*]] = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> [[TMP1]], i32* [[ADDR:%.*]])
; CHECK-NEXT:    [[TMP3:%.*]] = call <vscale x 8 x i1> @llvm.aarch64.sve.ptrue.nxv8i1(i32 1)
; CHECK-NEXT:    [[TMP4:%.*]] = call <vscale x 8 x i32> @llvm.aarch64.sve.ld1.nxv8i32(<vscale x 8 x i1> [[TMP3]], i32* [[ADDR]])
; CHECK-NEXT:    ret <vscale x 8 x i32> [[TMP4]]
;
  %1 = call <vscale x 4 x i1> @llvm.aarch64.sve.ptrue.nxv4i1(i32 1)
  %2 = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> %1, i32* %addr)
  %3 = call <vscale x 8 x i1> @llvm.aarch64.sve.ptrue.nxv8i1(i32 1)
  %4 = call <vscale x 8 x i32> @llvm.aarch64.sve.ld1.nxv8i32(<vscale x 8 x i1> %3, i32* %addr)
  ret <vscale x 8 x i32> %4
}

; Four calls to the SVE ptrue intrinsic. %7 is the most encompassing, and the others can be expressed as an
; SVE reinterprets of %7 via convert.{to,from}.svbool.
define <vscale x 16 x i32> @coalesce_test_multiple(i32* %addr) {
; CHECK-LABEL: @coalesce_test_multiple(
; CHECK-NEXT:    [[TMP1:%.*]] = tail call <vscale x 16 x i1> @llvm.aarch64.sve.ptrue.nxv16i1(i32 31)
; CHECK-NEXT:    [[TMP2:%.*]] = call <vscale x 16 x i1> @llvm.aarch64.sve.convert.to.svbool.nxv16i1(<vscale x 16 x i1> [[TMP1]])
; CHECK-NEXT:    [[TMP3:%.*]] = call <vscale x 8 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv8i1(<vscale x 16 x i1> [[TMP2]])
; CHECK-NEXT:    [[TMP4:%.*]] = call <vscale x 4 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv4i1(<vscale x 16 x i1> [[TMP2]])
; CHECK-NEXT:    [[TMP5:%.*]] = call <vscale x 2 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv2i1(<vscale x 16 x i1> [[TMP2]])
; CHECK-NEXT:    [[TMP6:%.*]] = call <vscale x 2 x i32> @llvm.aarch64.sve.ld1.nxv2i32(<vscale x 2 x i1> [[TMP5]], i32* [[ADDR:%.*]])
; CHECK-NEXT:    [[TMP7:%.*]] = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> [[TMP4]], i32* [[ADDR]])
; CHECK-NEXT:    [[TMP8:%.*]] = call <vscale x 8 x i32> @llvm.aarch64.sve.ld1.nxv8i32(<vscale x 8 x i1> [[TMP3]], i32* [[ADDR]])
; CHECK-NEXT:    [[TMP9:%.*]] = call <vscale x 16 x i32> @llvm.aarch64.sve.ld1.nxv16i32(<vscale x 16 x i1> [[TMP1]], i32* [[ADDR]])
; CHECK-NEXT:    ret <vscale x 16 x i32> [[TMP9]]
;
  %1 = tail call <vscale x 2 x i1> @llvm.aarch64.sve.ptrue.nxv2i1(i32 31)
  %2 = call <vscale x 2 x i32> @llvm.aarch64.sve.ld1.nxv2i32(<vscale x 2 x i1> %1, i32* %addr)
  %3 = tail call <vscale x 4 x i1> @llvm.aarch64.sve.ptrue.nxv4i1(i32 31)
  %4 = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> %3, i32* %addr)
  %5 = tail call <vscale x 8 x i1> @llvm.aarch64.sve.ptrue.nxv8i1(i32 31)
  %6 = call <vscale x 8 x i32> @llvm.aarch64.sve.ld1.nxv8i32(<vscale x 8 x i1> %5, i32* %addr)
  %7 = tail call <vscale x 16 x i1> @llvm.aarch64.sve.ptrue.nxv16i1(i32 31)
  %8 = call <vscale x 16 x i32> @llvm.aarch64.sve.ld1.nxv16i32(<vscale x 16 x i1> %7, i32* %addr)
  ret <vscale x 16 x i32> %8
}

; Two calls to the SVE ptrue intrinsic which are both of the same size. In this case, one should be identified
; as redundant and rewritten and an SVE reinterpret of the other via the convert.{to,from}.svbool intrinsics.
; This introduces a redundant conversion which will then be eliminated.
define <vscale x 4 x i32> @coalesce_test_same_size(i32* %addr) {
; CHECK-LABEL: @coalesce_test_same_size(
; CHECK-NEXT:    [[TMP1:%.*]] = tail call <vscale x 4 x i1> @llvm.aarch64.sve.ptrue.nxv4i1(i32 31)
; CHECK-NEXT:    [[TMP2:%.*]] = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> [[TMP1]], i32* [[ADDR:%.*]])
; CHECK-NEXT:    [[TMP3:%.*]] = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> [[TMP1]], i32* [[ADDR]])
; CHECK-NEXT:    ret <vscale x 4 x i32> [[TMP3]]
;
  %1 = tail call <vscale x 4 x i1> @llvm.aarch64.sve.ptrue.nxv4i1(i32 31)
  %2 = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> %1, i32* %addr)
  %3 = tail call <vscale x 4 x i1> @llvm.aarch64.sve.ptrue.nxv4i1(i32 31)
  %4 = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> %3, i32* %addr)
  ret <vscale x 4 x i32> %4
}

; Two calls to the SVE ptrue intrinsic, but neither can be eliminated; %1 is promoted to become %3, which
; means eliminating this call to the SVE ptrue intrinsic would involve creating a longer, irreducible chain of
; conversions. Better codegen is achieved by just leaving the ptrue as-is.
define <vscale x 8 x i16> @coalesce_test_promoted_ptrue(i32* %addr1, i16* %addr2) {
; CHECK-LABEL: @coalesce_test_promoted_ptrue(
; CHECK-NEXT:    [[TMP1:%.*]] = call <vscale x 8 x i1> @llvm.aarch64.sve.ptrue.nxv8i1(i32 31)
; CHECK-NEXT:    [[TMP2:%.*]] = call <vscale x 16 x i1> @llvm.aarch64.sve.convert.to.svbool.nxv8i1(<vscale x 8 x i1> [[TMP1]])
; CHECK-NEXT:    [[TMP3:%.*]] = call <vscale x 4 x i1> @llvm.aarch64.sve.ptrue.nxv4i1(i32 31)
; CHECK-NEXT:    [[TMP4:%.*]] = call <vscale x 16 x i1> @llvm.aarch64.sve.convert.to.svbool.nxv4i1(<vscale x 4 x i1> [[TMP3]])
; CHECK-NEXT:    [[TMP5:%.*]] = call <vscale x 8 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv8i1(<vscale x 16 x i1> [[TMP4]])
; CHECK-NEXT:    [[TMP6:%.*]] = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> [[TMP3]], i32* [[ADDR1:%.*]])
; CHECK-NEXT:    [[TMP7:%.*]] = call <vscale x 8 x i16> @llvm.aarch64.sve.ld1.nxv8i16(<vscale x 8 x i1> [[TMP5]], i16* [[ADDR2:%.*]])
; CHECK-NEXT:    [[TMP8:%.*]] = call <vscale x 8 x i16> @llvm.aarch64.sve.ld1.nxv8i16(<vscale x 8 x i1> [[TMP1]], i16* [[ADDR2]])
; CHECK-NEXT:    ret <vscale x 8 x i16> [[TMP8]]
;
  %1 = call <vscale x 4 x i1> @llvm.aarch64.sve.ptrue.nxv4i1(i32 31)
  %2 = call <vscale x 16 x i1> @llvm.aarch64.sve.convert.to.svbool.nxv4i1(<vscale x 4 x i1> %1)
  %3 = call <vscale x 8 x i1> @llvm.aarch64.sve.convert.from.svbool.nxv4i1(<vscale x 16 x i1> %2)

  %4 = call <vscale x 4 x i32> @llvm.aarch64.sve.ld1.nxv4i32(<vscale x 4 x i1> %1, i32* %addr1)
  %5 = call <vscale x 8 x i16> @llvm.aarch64.sve.ld1.nxv8i16(<vscale x 8 x i1> %3, i16* %addr2)

  %6 = call <vscale x 8 x i1> @llvm.aarch64.sve.ptrue.nxv8i1(i32 31)
  %7 = call <vscale x 8 x i16> @llvm.aarch64.sve.ld1.nxv8i16(<vscale x 8 x i1> %6, i16* %addr2)
  ret <vscale x 8 x i16> %7
}