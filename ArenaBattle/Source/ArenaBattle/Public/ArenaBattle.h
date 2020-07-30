// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

// ArenaBattle이라는 로그 카테고리 생성
// cpp에 DECLARE_LOG_CATEGORY와 페어가 되어야 한다
// EXTERN - 즉, 모든 곳에서 접근 가능
DECLARE_LOG_CATEGORY_EXTERN(ArenaBattle, Log, All);

#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define ABLOG_S(Verbosity) UE_LOG(ArenaBattle, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)
#define ABLOG(Verbosity, Format, ...) UE_LOG(ArenaBattle, Verbosity, TEXT("%s %s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

// 런타임 시 문제가 생길 때 에러로그를 뿌리고 바로 함수를 반환
#define ABCHECK(Expr, ...) { if(!(Expr)) { ABLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; }}

// 매크로 할 때 한 줄로 해야하는 듯? 한 줄 안하니 컴파일 오류 뜸