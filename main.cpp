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

struct ntfc_header_atribut
{
    uint32_t type; //тип атрибута
    uint32_t lenght; // длина атрибута
    uint8_t flag; // флаг нерезидентного атрибута
    uint8_t lenght_name; // длина имени
    uint16_t offset_name; // смещение имени
    uint16_t flag_pack; // флаг упаковки
    uint16_t indificator; // индификатор
    uint32_t size_in_byte; // размер содержимого в байтах
    uint16_t offset_content; // смещение содержимого атрибута
    uint8_t flag_indexing; // флаг индексирования
    uint8_t context; //заполнение
};

string what_is_struct(uint32_t type){
    if (type == 0x10) return "$STANDART_INFORMATION";
    else if (type == 0x20) return "$ATTRIBUTE_LIST";
    else if (type == 0x30) return "$FILE_NAME";
    else if (type == 0x40) return "$OBJECT_ID";
    else if (type == 0x50) return "$SECURITY_DESCRIPTOR";
    else if (type == 0x80) return "$DATA";
    else if (type == 0x90) return "$INDEX_ROOT";
    else if (type == 0xA0) return "$INDEX_ALLOCATION";
    else if (type == 0xB0) return "$BITMAP";
    else if (type == 0x100) return "$LOGGED_UTILITY_STREAM";

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
        cout << endl;
        ntfc_file.seekg(0, ios::beg);


        // Выделить память и считать данные
        vector<char> ntfc_data(fileSize, 0);
        ntfc_file.read(ntfc_data.data(), fileSize);

        ntfc_header* n_header = reinterpret_cast<ntfc_header*>(&ntfc_data[0]);


        // Вывести жирные поля структуры-2 пункт
        displayHeader(n_header);


        // 3 пункт Вывод названия и типа атрибутов
        uint16_t offset_f_atribute;
        offset_f_atribute = int(n_header->offset_atribute);
        ntfc_header_atribut* n_atribut = reinterpret_cast<ntfc_header_atribut*>(&ntfc_data[offset_f_atribute]);

        while (n_atribut->type != 0xFFFFFFFF){

            cout << endl << "Name of atribut:" << what_is_struct(n_atribut->type) << endl;
            cout << "Type of atribut: 0x" << hex << n_atribut->type << endl;
            offset_f_atribute += (n_atribut->lenght);
            n_atribut = reinterpret_cast<ntfc_header_atribut*>(&ntfc_data[offset_f_atribute]);
        }

    }

    return 0;
}
