#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <cstdint>

using namespace std;


#pragma pack(push, 1)

// Объявить структуру - 1 пункт
struct ntfc_header
{
    uint32_t type; //тип файла
    uint16_t offsetN; // смещение N массивы
    uint16_t size_array; // размер массива
    uint64_t number_in_lsn; // номер посследовательности в LSN
    uint16_t number_sec_in_lsn; // номер сектора в MFT
    uint16_t size_links; // кол-во ссылок

    uint16_t offset_atribute;// смещение атрибута
    uint16_t flags; //флаги
    uint32_t real_size;// реальный размер
    uint32_t selected_size; //выденный размер
    uint64_t file_links;//файловая ссылка на базовую запись
    uint32_t indificator;//индификатор

    uint32_t number_file_recording;//номер файловой записи

};

// 2 пункт - вывод жирных полей
void displayHeader(ntfc_header* header) {
    cout << "Indificator: 0x" << hex << int(header->type) << endl;
    cout << "Offset first atribute: 0x" << hex << int(header->offset_atribute) << endl;
    cout << "Number of file recording: 0x" << hex << int(header->number_file_recording) << endl;
}

// Восстановить выравнивание
#pragma pack(pop)
uint16_t offset_f_atribute;
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
        cout << endl;
        ntfc_file.seekg(0, ios::beg);


        // Выделить память и считать данные
        vector<char> ntfc_data(fileSize, 0);
        ntfc_file.read(ntfc_data.data(), fileSize);

        ntfc_header* n_header = reinterpret_cast<ntfc_header*>(&ntfc_data[0]);


        // Вывести жирные поля структуры-2 пункт
        displayHeader(n_header);

        }

    return 0;
}
