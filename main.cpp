#include <iostream>
#include <fstream>

using namespace std;

int main() {
    string save_file_name;
    fstream save_file;
    int new_money_count;

    cout << "Podaj nazwe pliku save do GTA SA: ";
    cin >> save_file_name;

    save_file.open(save_file_name, ios::binary | ios::in | ios::out);
    if (save_file.bad()) {
        cout << "Plik nie moze zostac otworzony!";
        return 0;
    }

    cout << "Podaj nowa kwote: ";
    cin >> new_money_count;

    char block_struct[] = {0x42, 0x4C, 0x4F, 0x43, 0x4B};    // [BLOCK]
    int current_block = -1;
    int how_many_chars_are_correct = 0;

    save_file.seekg(0, ios::end);
    streampos endOfFile = save_file.tellg();
    save_file.seekg(0, ios::beg);
    cout << "Dlugosc pliku: " << endOfFile << endl;
    int lenghtOfFile = (endOfFile);

    char reader;
    int begin_of_money_block;
    for (int current_character = 0; current_character<=lenghtOfFile; current_character++) {
        save_file.seekg(current_character);
        save_file.read(&reader, 1);


        if (reader == block_struct[how_many_chars_are_correct]) {
            how_many_chars_are_correct++;
        } else {
            how_many_chars_are_correct = 0;
        }

        if (how_many_chars_are_correct == 5) {
            current_block++;
            how_many_chars_are_correct = 0;
        }

        if (current_block == 15) {
            begin_of_money_block = save_file.tellg();
            save_file.seekg(begin_of_money_block+4);
           // char old_money_count[4];
            //save_file.read(old_money_count, sizeof(old_money_count));
            //cout << "Stara wartosc: " << (int)old_money_count[1] << endl;
            save_file.write((char*)&new_money_count, sizeof(new_money_count));
            save_file.seekg(begin_of_money_block+10);
            save_file.write((char*)&new_money_count, sizeof(new_money_count));
            break;
        }

    }

    // Teraz musimy zupdateowac checksum (4 ostatnie bajty save'a)
    int checksum = 0;
    for (int current_character = 0; current_character<lenghtOfFile-4; current_character++) {
        save_file.seekg(current_character);
        save_file.read(&reader, 1);
        checksum += (unsigned char)reader;
    }

    save_file.seekg(lenghtOfFile-4);
    save_file.write((char*)&checksum, sizeof(checksum));

    cout<<"checksum: "<<checksum;
    save_file.close();

    return 0;
}
