#pragma once
#include <Windows.h>
#include <iostream>
using namespace std;
//��������� �������
void Offset(unsigned int *, const char *);
void Check(const wchar_t * fileName);
void Retrieve(const wchar_t * imageName, const wchar_t *textName);
void Embed(const wchar_t * imageName, const wchar_t *textName);
//���������
const char cMask = 0x01;//��� ����������� �� ������ ����
const char cMask_2 = 0xFE;//��� ��������� �������� ���� ����� �����������
const char cTag[5] = "MARK";

