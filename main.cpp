#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <cstdint> // Это для использования обозначений целых чисел с указанием их размера

using namespace std;

// Установить 1-байтовое выравнивание
#pragma pack(push, 1)

// Объявить структуру
struct ntfc_header
{
    char type[4]; //тип файла
    uint16_t offsetN; // смещение N массивы
    uint16_t size_array; // размер массива
    uint64_t number_in_lsn; // номер посследовательности
    uint16_t number_sec_in_lsn; // номер сектора
    uint16_t size_links; // кол-во ссылок

    uint16_t offset_attribute;
    uint16_t flags;
    uint32_t real_size;
    uint32_t selected_size = 1024;
    uint64_t file_links;
    uint32_t indificator;

    uint32_t number_file_recording;


    // Дальше просто демонстрация
    // char padding[8]; // Пропуск 8 байтов (просто чтобы показать)
    // int32_t anotherInt; // Еще целое 4-байтовое число со знаком
};

void displayHeader(ntfc_header* header) {
    cout << "Indificator: " << string(header->type) << endl;
    cout << "Offset first attribute: " << int(header->offset_attribute) << endl;
    cout << "Number of file recording: " << int(header->number_file_recording) << endl;
}

// Восстановить выравнивание
#pragma pack(pop)

int main()
{
    // Открыть файл в двоичном режиме
    ifstream ntfc_file("File_record.dat", ios::binary);

    // Убедиться, что открылся
    if(ntfc_file.is_open())
    {
        // Определить размер файла
        ntfc_file.seekg(0, ios::end);
        int fileSize = ntfc_file.tellg();
        cout << "File size: " << fileSize << endl;

        // Снова спозиционироваться в начало файла
        ntfc_file.seekg(0, ios::beg);

        vector<char> signature(4);
        ntfc_file.read(signature.data(), 4);
        string signature_str(signature.data());

        // Выделить память
        vector<char> ntfc_data(fileSize, 0);

        // Считать данные
        ntfc_file.read(ntfc_data.data(), fileSize);


        //
        ntfc_header* header;
        ntfc_file.read(reinterpret_cast<char*>(&header), fileSize);
        //


        // Инициализировать указатель на структуру
        // (по смещению 7, потому что надо пропустить сигнатуру длиной 7 байт)
        ntfc_header* n_type = reinterpret_cast<ntfc_header*>(&ntfc_data);

        // rar_header* - тип переменной
        // p_header - имя переменной
        // &rar_data[7] - указатель на байт массива с индексом 7
        // reinterpret_cast<rar_header*> - приведение типа к rar_header*

        // Вывести жирные поля структур
        displayHeader(n_type);
        //cout << signature_str;
        //cout << ntfc_data[];

        // Вывод в шестнадцатеричном представлении (hex)
        //cout << "Type file: 0x" << int(si->type) << endl;

        // Вывод в десятичном представлении (dec)
        //cout << "Header size: " << dec << int(p_header->header_size) << endl;





    }

    return 0;
}
