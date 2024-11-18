; ModuleID = 'MyProgram'
source_filename = "MyProgram"

@first = global i32 5
@second = global i32 6
@ans = global i32 99

declare void @PrintInt(i32)

declare void @PrintReal(double)

declare void @PrintBoolean(i1)

define i32 @sum(i32 %a, i32 %b) {
entry:
  %s = alloca i32, align 4
  %addtmp_simple = add i32 %a, %b
  store i32 %addtmp_simple, ptr %s, align 4
  %s1 = load i32, ptr %s, align 4
  ret i32 %s1
}

define i1 @firstmoresecond(i32 %a, i32 %b) {
entry:
  %ans = alloca i1, align 1
  %cmpgt_relation = icmp sgt i32 %a, %b
  store i1 %cmpgt_relation, ptr %ans, align 1
  %ans1 = load i1, ptr %ans, align 1
  ret i1 %ans1
}

define i32 @comparator(i32 %a, i32 %b) {
entry:
  %max = alloca i32, align 4
  store i32 %b, ptr %max, align 4
  %cmpgt_relation = icmp sgt i32 %a, %b
  br i1 %cmpgt_relation, label %then, label %else

then:                                             ; preds = %entry
  store i32 %a, ptr %max, align 4
  br label %ifcont

else:                                             ; preds = %entry
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %max1 = load i32, ptr %max, align 4
  ret i32 %max1
}

define i32 @main() {
entry:
  %first = load i32, ptr @first, align 4
  %second = load i32, ptr @second, align 4
  %ans = load i32, ptr @ans, align 4
  %calltmp_routine = call i32 @sum(i32 %first, i32 %second)
  call void @PrintInt(i32 %calltmp_routine)
  %calltmp_routine1 = call i32 @comparator(i32 %first, i32 %second)
  call void @PrintInt(i32 %calltmp_routine1)
  %calltmp_routine2 = call i1 @firstmoresecond(i32 %first, i32 %second)
  call void @PrintInt(i1 %calltmp_routine2)
  call void @PrintInt(i32 %ans)
  ret i32 0
}
