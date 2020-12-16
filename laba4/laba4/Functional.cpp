#include "Header.h"
#include <Windows.h>
#include <iostream>
using namespace std;
void Offset(unsigned int * pOffset, const char * Buffer)
{
	*pOffset = 0; //Получение смещения
	*pOffset = *pOffset | Buffer[13];
	*pOffset = *pOffset << 8;
	*pOffset = *pOffset | Buffer[12];
	*pOffset = *pOffset << 8;
	*pOffset = *pOffset | Buffer[11];
	*pOffset = *pOffset << 8;
	*pOffset = *pOffset | Buffer[10];
}
void Check(const wchar_t * fileName)  //Проверка на наличие секретного сообщения
{
	HANDLE hFile;
	char * Buff;
	DWORD dwBytes;
	//Открываем изображение
	if (INVALID_HANDLE_VALUE == (hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
		cout << "Can't open file\n";
	//Читаем информацию о файле и 40 байт растра
	Buff = new char[100];
	ReadFile(hFile, Buff, 100, &dwBytes, NULL);//Берём 100 первых байт
	unsigned int uiOffset;
	Offset(&uiOffset, Buff);
	//Формирование метки из изображения
	//от смещения — 40 байт
	unsigned char cTag_2[5] = { 0 };
	//Получение значений цветов и формирование новых
	char cImageByte;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			char cImageBit = 0;
			cImageByte = Buff[uiOffset];
			cImageBit = cImageByte & cMask;
			cImageBit = cImageBit << 7;
			cTag_2[i] = cTag_2[i] >> 1;
			cTag_2[i] = cTag_2[i] | cImageBit;
			uiOffset++;
		}
	}
	char cTagTemp[5] = { cTag_2[0], cTag_2[1], cTag_2[2], cTag_2[3], cTag_2[4] };//для сравнение с меткой
	if (strcmp(cTag, cTagTemp))
	{
		printf("No secret text here.\n");
	}
	else
		printf("Secret text.\n");
	CloseHandle(hFile);
	delete[] Buff;
}
void Retrieve(const wchar_t * imageName, const wchar_t *textName)
{
	HANDLE hFile;
	if (INVALID_HANDLE_VALUE == (hFile = CreateFile(imageName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
		cout << "Can't open file\n";
	DWORD dwBytes = 0, BuffSize = 0, dwBytes1 = 0;
	BuffSize = GetFileSize(hFile, NULL);
	char * Buff = new char[BuffSize / sizeof(char)];
	ReadFile(hFile, Buff, BuffSize, &dwBytes, NULL);
	unsigned int uiOffset;
	Offset(&uiOffset, Buff);
	char cTagTemp[5], cImageByte;
	unsigned char cTag_2[5] = { 0 };
	//Проверка метки
	for (int i = 0; i < 5; i++)
		cTag_2[i] = 0;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			char cImageBit = 0;
			cImageByte = Buff[uiOffset];
			cImageBit = cImageByte & cMask;
			cImageBit = cImageBit << 7;
			cTag_2[i] = cTag_2[i] >> 1;
			cTag_2[i] = cTag_2[i] | cImageBit;
			uiOffset++;
		}
	}
	for (int i = 0; i < 5; i++)
		cTagTemp[i] = cTag_2[i];
	if (!strcmp(cTag, cTagTemp))//Если метка есть узнаём размер текста и восстанавливаем его
	{
		//Считываем размер
		unsigned char cTextSize[4] = { 0 };
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				char cImageBit = 0;
				cImageByte = Buff[uiOffset];
				cImageBit = cImageByte & cMask;
				cImageBit = cImageBit << 7;
				cTextSize[i] = cTextSize[i] >> 1;
				cTextSize[i] = cTextSize[i] | cImageBit;
				uiOffset++;
			}
		}
		//Преобразование в int cTextSize
		int iTextSize = 0;
		for (int i = 3; i > -1; i--)
		{
			iTextSize = iTextSize << 8;
			iTextSize = iTextSize | cTextSize[i];
		}
		//Отводим память под текст
		char * Buffer2 = new char[iTextSize];
		//Считываем текст
		for (int i = 0; i < iTextSize; i++)
		{
			unsigned char BufferTemp = 0;
			for (int j = 0; j < 8; j++)
			{
				char cImageBit = 0;
				cImageByte = Buff[uiOffset];
				cImageBit = cImageByte & cMask;
				cImageBit = cImageBit << 7;
				BufferTemp = BufferTemp >> 1;
				BufferTemp = BufferTemp | cImageBit;
				uiOffset++;
			}
			Buffer2[i] = BufferTemp;
		}
		HANDLE hText;
		//Запись в файл
		if (INVALID_HANDLE_VALUE == (hText = CreateFile(textName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)))
			cout << "Can't create file\n";
		WriteFile(hText, Buffer2, iTextSize, &dwBytes1, NULL);
		CloseHandle(hText);
		delete[] Buffer2;
	}
	else
	{
		cout << "Nothing to retrieve.\n";
		exit(2);
	}
	CloseHandle(hFile);
	delete[] Buff;
}
void Embed(const wchar_t * imageName, const wchar_t *textName)
{
	HANDLE hFile;
	//HANDLE hFile1;
	if (INVALID_HANDLE_VALUE == (hFile = CreateFile(imageName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
		cout << "Can't open the image\n";
	DWORD dwBytes = 0, BuffSize = 0, dwBytes_1 = 0;
	BuffSize = GetFileSize(hFile, NULL);
	char *Buff = new char[BuffSize / sizeof(char)];
	ReadFile(hFile, Buff, BuffSize, &dwBytes, NULL);
	//Открытие текста
	HANDLE hText;
	if (INVALID_HANDLE_VALUE == (hText = CreateFile(textName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
		cout << "Can't open the text file\n";
	//Считывание текста в буфер
	DWORD dwBytes_2 = 0, BuffSize_2 = 0;
	BuffSize_2 = GetFileSize(hText, NULL);
	char *Buff_2 = new char[BuffSize_2 / sizeof(char)];
	ReadFile(hText, Buff_2, BuffSize_2, &dwBytes_2, NULL);
	//Запись текста в изображение
	//Получение значения смещения
	unsigned int uiOffset = 0;
	Offset(&uiOffset, Buff);
	//Получение значений цветов и формирование новых
	char cImageByte;
	//Для обработки текста
	char cTextByte = 0, cTextBit = 0;
	unsigned int uiTextOffset = 0;
	//Проверка размера
	if (BuffSize < (BuffSize_2 * 8) + 88)
	{
		cout << "Text do not fit into the image.\n";
		exit(3);
	}
	//Запись метки в изображение
	for (int i = 0; i < 5; i++)
	{
		char temp = cTag[i], tempbit = 0;
		for (char j = 0; j < 8; j++)
		{
			cImageByte = Buff[uiOffset];
			//и запись в него бита текста
			tempbit = temp & cMask;
			temp = temp >> 1;
			if (cImageByte % 2 == 0)
				cImageByte = cImageByte | tempbit;
			else
			{
				cImageByte = cImageByte & cMask_2;
				cImageByte = cImageByte | tempbit;
			}
			Buff[uiOffset] = cImageByte;
			uiOffset++;
		}
	}
	//запись размера текстового файла в изображение
	//32 бита => 32 байта
	for (char j = 0; j < 32; j++)
	{
		//int temp = BufSize2;
		char tempbit = 0;
		cImageByte = Buff[uiOffset];
		//и запись в него бита текста
		tempbit = BuffSize_2 & cMask;
		BuffSize_2 = BuffSize_2 >> 1;//изменение BufSize2
		if (cImageByte % 2 == 0)
			cImageByte = cImageByte | tempbit;
		else
		{
			cImageByte = cImageByte & cMask_2;
			cImageByte = cImageByte | tempbit;
		}
		Buff[uiOffset] = cImageByte;
		uiOffset++;
	}
	//восстановление BufSize2
	BuffSize_2 = GetFileSize(hText, NULL);
	//Цикл записи текста в изображене
	for (unsigned int i = 0; i < BuffSize_2; i++)
	{
		cTextByte = Buff_2[uiTextOffset];//один байт текста на восемь байт изображения
		for (char j = 0; j < 8; j++)
		{
			//Получение байта изображения
			cImageByte = Buff[uiOffset];
			//и запись в него бита текста
			cTextBit = cTextByte & cMask;
			cTextByte = cTextByte >> 1;
			if (cImageByte % 2 == 0)
				cImageByte = cImageByte | cTextBit;
			else
			{
				cImageByte = cImageByte & cMask_2;//обнуляем младший бит для правильной записи
				cImageByte = cImageByte | cTextBit;
			}
			Buff[uiOffset] = cImageByte;
			uiOffset++;
		}
		uiTextOffset++;
	}
	//Конец цикла
	CloseHandle(hFile);
	if (INVALID_HANDLE_VALUE == (hFile = CreateFile(imageName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
		cout << "Writing in the file failed.\n";
	//Запись в файл
	WriteFile(hFile, Buff, BuffSize, &dwBytes_1, NULL);
	//Закрытие HANDLE
	CloseHandle(hText);
	CloseHandle(hFile);
	delete Buff;
	delete Buff_2;
}
