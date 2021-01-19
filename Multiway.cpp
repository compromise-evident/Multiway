/// Multiway - file encryption without reversal shortcuts.
/// Nikolay Valentinovich Repnitskiy - License: WTFPLv2+ (wtfpl.net.)


/* Version 1. Any key tried is not rejected and successfully decrypts any file,
giving mostly useless output. You may rename then decrypt files, or decrypt what
has never been encrypted--hiding n in plain sight. All cryptanalysis necessarily
begins with guess keys, hence the lack of shortcuts--leading to keys. Data to be
encrypted is unspecified meaning there are no expected identifying properties to
look out for when attacking with brute force. You must code cryptanalytic search
priorities to discard random output and store what appears ordered or plausible.
Multiway begins with perfect secrecy of the One-time pad's all-way function then
steps down to decryption difficulty of multi-way functions by evolving your key.
Cipherfiles of plainfiles are equal in size, use schemeOTP if sending documents.
--------------------------------------------------------------------------------
Unsuspecting deniability -  If you and some party share a password, you can take
numerous photos or any files,  DECRYPT  them all with that single password, then
send only the file whose output produced intended bytes within the predetermined
location/byte index (all files are secretly text files stored in byte quantity.)
--------------------------------------------------------------------------------
Proof:  github.com/compromise-evident/OTP   Light background for the unprepared:
Multiway produces functions--solutions to which are easily verifiable--but never
easily discoverable. This check-marks one class of functions, but not the other.
Namely P and NP. That discrepancy means this function is listed under NP but not
under P therefore the two classes mismatch--mathematically represented as  P≠NP.
--------------------------------------------------------------------------------
How to run the program  -  Software package repositories for GNU+Linux operating
systems have all the tools you can imagine. Open a terminal and use this command
as root to install Geany and g++ on your computer: apt install geany g++   Geany
is a fast & lightweight text editor and Integrated Development Environment where
you can write and run code. g++ is the GNU compiler for C++ which allows written
code to run. The compiler operates in the background and displays errors in your
code as you will see in the lower Geany box. Make a new folder somewhere on your
machine. Paste this code into Geany. For clarity in auditing, enable indentation
guides: go to View >> Show Indentation Guides. Save the document as anything.cpp
within the newly-created folder. Use these shortcuts to run the program: F9, F5.
You may paste over this code with other .cpp files, or repeat in a new tab.  */

#include <fstream>
#include <iostream>
using namespace std;

int main()
{	ifstream in_stream;
	ofstream out_stream;
	
	cout << "\n(Multi-way function)\n\n"
	
	     << "(1) Encrypt file\n"
	     << "(2) Decrypt file\n\n"
	
	     << "Enter option: ";
	
	int user_option; cin >> user_option;
	if((user_option != 1) && (user_option != 2)) {cout << "\nInvalid, program ended.\n"; return 0;}
	
	//Gets path to file from user.
	cout << "\nDrag & drop file into terminal or enter path:\n\n";
	char catching_new_line[1];
	char  path_to_file[10000];
	for(int a = 0; a < 10000; a++) {path_to_file[a] = '\0';} //Fills path_to_file[] with null.
	cin.getline(catching_new_line, 1);
	cin.getline(path_to_file, 10000);
	if(path_to_file[0] == '\0') {cout << "\nNo path given.\n"; return 0;}
	
	//Fixes path to file if drag & dropped (removes single quotes for ex:)   '/home/nikolay/my documents/hush hush.bmp'
	if(path_to_file[0] == '\'')
	{	for(int a = 0; a < 10000; a++)
		{	path_to_file[a] = path_to_file[a + 1];
			if(path_to_file[a] == '\'')
			{	path_to_file[a] = '\0';
				path_to_file[a + 1] = '\0';
				path_to_file[a + 2] = '\0';
				break;
			}
		}
	}
	
	//Checks if file exists (failure can be bad path info as well.)
	in_stream.open(path_to_file);
	if(in_stream.fail() == true) {in_stream.close(); cout << "\n\nNo such file or directory.\n";             return 0;}
	char sniffed_one_file_character;
	in_stream.get(sniffed_one_file_character);
	if(in_stream.eof() == true) {in_stream.close();  cout << "\n\nNothing to process, the file is empty.\n"; return 0;}
	in_stream.close();
	
	//Gets location of the first encountered end-null coming from the left in path_to_file[].
	int path_to_file_null_bookmark;
	for(int a = 0; a < 10000; a++) {if(path_to_file[a] == '\0') {path_to_file_null_bookmark = a; break;}}
	
	//Gets key from the user.
	if(user_option == 1)
	{	cout << "\nEnter a password (1,000,000 characters max, input is invisible while typing.)\n"
		     << "Warning: very long passwords result in impractical wait times, go for ~100.\n\n";
	}
	else {cout << "\nEnter decryption password (input is invisible while typing.)\n\n";}
	
	system("stty -echo"); //Renders keyboard input invisible.
	char key[1000001];
	for(int a = 0; a < 1000001; a++) {key[a] = '\0';}
	cin.getline(key,   1000001);
	key[1000000] = '\0';
	system("stty echo"); //Restores keyboard input visibility.
	if(key[0] == '\0') {cout << "\nNothing entered.\n"; return 0;}
	
	//Constructs actual key using the given user key (fills with randomness 0 - 255, later converted to (-128 - 127) upon writing to files.)
	cout << "\nBuilding key...\n"; //Unfortunately, progress indicators must have new lines else nothing is printed until everything is done.
	unsigned char actual_key[1000000] = {0};
	int temp_integer_arithmetic;
	for(int a = 0; key[a] != '\0'; a++) //Constructively applies random digits to based on the key (key is used for seeds here.).
	{	srand(key[a]);                  //WRITES ALTERNATING BETWEEN LEFT TO RIGHT & RIGHT TO LEFT. Alternation is based on the seeds.
		
		if((key[a] % 2) == 0)
		{	for(int b = 0; b < 1000000; b++) //WRITES LEFT TO RIGHT.
			{	temp_integer_arithmetic = actual_key[b];
				temp_integer_arithmetic += (rand() % 256);
				actual_key[b] = (temp_integer_arithmetic % 256);
			}
		}
		else
		{	for(int b = 999999; b >= 0; b--) //WRITES RIGHT TO LEFT.
			{	temp_integer_arithmetic = actual_key[b];
				temp_integer_arithmetic += (rand() % 256);
				actual_key[b] = (temp_integer_arithmetic % 256);
			}
		}
	}
	
	//Adding additional randomness in actual key.
	unsigned int seeds_sum = 0;
	for(int a = 0; key[a] != '\0'; a++) {seeds_sum += key[a];}
	srand(seeds_sum); //A new seed comes from the sum of ALL key items.
	for(int a = 0; a < 1000000; a++) //WRITES LEFT TO RIGHT.
	{	temp_integer_arithmetic = actual_key[a];
		temp_integer_arithmetic += (rand() % 256);
		actual_key[a] = (temp_integer_arithmetic % 256);
	}
	
	//Again, adding additional randomness in actual key.
	seeds_sum = 0;
	for(int a = 0; key[a] != '\0'; a += 2) {seeds_sum += key[a];}
	srand(seeds_sum); //A new seed comes from the sum of EVERY OTHER key item.
	for(int a = 999999; a >= 0; a--) //WRITES LEFT TO RIGHT.
	{	temp_integer_arithmetic = actual_key[a];
		temp_integer_arithmetic += (rand() % 256);
		actual_key[a] = (temp_integer_arithmetic % 256);
	}
	
	//Constructs the transformation determinant using the actual key. This thing will evolve the actual key when
	//encrypting and decrypting. And it renews itself then again, by copying distributed items in the actual key.
	//This renewal happens every time the actual key is used up. You will get a new (transformed) actual key for
	//every megabyte that is encrypted. The actual key is one megabyte--hot zones require wide search priorities.
	unsigned char transformation_determinant[100] = {0};
	int actual_key_hopping_read_bookmark = 0;
	for(int a = 0; a < 100; a++)
	{	transformation_determinant[a] = actual_key[actual_key_hopping_read_bookmark];
		actual_key_hopping_read_bookmark += 10000;
	}
	
	//Gets a copy of path_to_file[] in case overwriting raw file.
	char path_to_file_copy_if_overwriting_raw[10000];
	for(int a = 0; a < 10000; a++) {path_to_file_copy_if_overwriting_raw[a] = path_to_file[a];}
	long long file_size_for_overwriting = 0;
	
	
	
	
	
	//______________________________________________________Encrypt___________________________________________________//
	if(user_option == 1)
	{	cout << "Encrypting...\n";
		
		//Prepares two file streams (read with one, write with another while both open--dynamic encryption.)
		in_stream.open(path_to_file);
		path_to_file[path_to_file_null_bookmark    ] = '.'; //Appending .mway to file name in path to file.
		path_to_file[path_to_file_null_bookmark + 1] = 'm';
		path_to_file[path_to_file_null_bookmark + 2] = 'w';
		path_to_file[path_to_file_null_bookmark + 3] = 'a';
		path_to_file[path_to_file_null_bookmark + 4] = 'y';
		out_stream.open(path_to_file);
		
		//Encrypts file dynamically.
		char temp_file_byte;
		int  temp_file_byte_normal;
		int  one_million_counter = 0;
		in_stream.get(temp_file_byte);
		for(; in_stream.eof() == false;)
		{	temp_file_byte_normal = temp_file_byte;
			if(temp_file_byte_normal < 0) {temp_file_byte_normal += 256;}
			temp_file_byte_normal += actual_key[one_million_counter]; ///Encrypting.
			temp_file_byte_normal %= 256;
			
			//Writing encrypted bytes to file.
			if(temp_file_byte_normal < 128) {out_stream.put(temp_file_byte_normal      );}
			else                            {out_stream.put(temp_file_byte_normal - 256);}
			
			file_size_for_overwriting++;
			one_million_counter++;
			
			//Transforms actual key then establishes a new transformation determinant form new actual key.
			if(one_million_counter == 1000000)
			{	for(int a = 0; a < 100; a++)              //Constructively applies random digits to based on the key (key is used for seeds here.).
				{	srand(transformation_determinant[a]); //WRITES ALTERNATING BETWEEN LEFT TO RIGHT & RIGHT TO LEFT. Alternation is based on the seeds.
					
					if((transformation_determinant[a] % 2) == 0)
					{	for(int b = 0; b < 1000000; b++) //WRITES LEFT TO RIGHT.
						{	temp_integer_arithmetic = actual_key[b];
							temp_integer_arithmetic += (rand() % 256);
							actual_key[b] = (temp_integer_arithmetic % 256);
						}
					}
					else
					{	for(int b = 999999; b >= 0; b--) //WRITES RIGHT TO LEFT.
						{	temp_integer_arithmetic = actual_key[b];
							temp_integer_arithmetic += (rand() % 256);
							actual_key[b] = (temp_integer_arithmetic % 256);
						}
					}
				}
				
				//Adding additional randomness in actual key.
				seeds_sum = 0;
				for(int a = 0; a < 100; a++) {seeds_sum += transformation_determinant[a];}
				srand(seeds_sum); //A new seed comes from the sum of ALL items in the transformation determinant.
				for(int a = 0; a < 1000000; a++) //WRITES LEFT TO RIGHT.
				{	temp_integer_arithmetic = actual_key[a];
					temp_integer_arithmetic += (rand() % 256);
					actual_key[a] = (temp_integer_arithmetic % 256);
				}
				
				//Again, adding additional randomness in actual key.
				seeds_sum = 0;
				for(int a = 0; a < 100; a += 2) {seeds_sum += transformation_determinant[a];}
				srand(seeds_sum); //A new seed comes from the sum of EVERY OTHER item in the transformation determinant.
				for(int a = 999999; a >= 0; a--) //WRITES LEFT TO RIGHT.
				{	temp_integer_arithmetic = actual_key[a];
					temp_integer_arithmetic += (rand() % 256);
					actual_key[a] = (temp_integer_arithmetic % 256);
				}
				
				//Constructs a new transformation determinant using the new actual key.
				actual_key_hopping_read_bookmark = 0;
				for(int a = 0; a < 100; a++)
				{	transformation_determinant[a] = actual_key[actual_key_hopping_read_bookmark];
					actual_key_hopping_read_bookmark += 10000;
				}
				
				one_million_counter = 0; //Resetting write-chunk (every key and its transformation from there is 1MB.)
			}
			
			in_stream.get(temp_file_byte);
		}
		
		in_stream.close();
		out_stream.close();
		
		cout << "\nDone! Cipherfile now resides in the given directory.";
	}
	
	
	
	
	
	//______________________________________________________Decrypt___________________________________________________//
	if(user_option == 2)
	{	cout << "Decrypting...\n";
		
		//Prepares two file streams (read with one, write with another while both open--dynamic decryption.)
		in_stream.open(path_to_file);
		if((path_to_file[path_to_file_null_bookmark - 5] == '.')
		&& (path_to_file[path_to_file_null_bookmark - 4] == 'm')
		&& (path_to_file[path_to_file_null_bookmark - 3] == 'w')
		&& (path_to_file[path_to_file_null_bookmark - 2] == 'a')
		&& (path_to_file[path_to_file_null_bookmark - 1] == 'y'))
		{
			path_to_file[path_to_file_null_bookmark - 5] = '\0'; //Truncates .mway if present. This one line will suffice as
			path_to_file[path_to_file_null_bookmark - 4] = '\0'; //the system reads name until null, however, this is complete.
			path_to_file[path_to_file_null_bookmark - 3] = '\0';
			path_to_file[path_to_file_null_bookmark - 2] = '\0';
			path_to_file[path_to_file_null_bookmark - 1] = '\0';
		}
		else
		{	path_to_file[path_to_file_null_bookmark    ] = '.'; //Appends .custom if .mway not present. Here, the user is decrypting
			path_to_file[path_to_file_null_bookmark + 1] = 'c'; //something completely renamed or that which has never been encrypted.
			path_to_file[path_to_file_null_bookmark + 2] = 'u'; //(Unsuspecting deniability and secrecy for sharing n in plain sight.)
			path_to_file[path_to_file_null_bookmark + 3] = 's';
			path_to_file[path_to_file_null_bookmark + 4] = 't';
			path_to_file[path_to_file_null_bookmark + 5] = 'o';
			path_to_file[path_to_file_null_bookmark + 6] = 'm';
		}
		out_stream.open(path_to_file);
		
		//Decrypts file dynamically.
		char temp_file_byte;
		int  temp_file_byte_normal;
		int  one_million_counter = 0;
		in_stream.get(temp_file_byte);
		for(; in_stream.eof() == false;)
		{	temp_file_byte_normal = temp_file_byte;
			if(temp_file_byte_normal < 0) {temp_file_byte_normal += 256;}
			
			///Decrypting. The following formula helps extract plaintext quickly.
			/*_____________________________________________ ________________________________________________
			|                                              |                                                |
			|          if sub-key <= cipherfile            |                     else                       |
			|   then plainfile = (cipherfile - sub-key)    |    plainfile = ((256 - sub-key) + cipherfile)  |
			|______________________________________________|_______________________________________________*/
			if(actual_key[one_million_counter] <= temp_file_byte_normal)
			{	temp_file_byte_normal = (temp_file_byte_normal - actual_key[one_million_counter]);
			}
			else
			{	temp_file_byte_normal = ((256 - actual_key[one_million_counter]) + temp_file_byte_normal);
			}
			
			//Writing decrypted bytes to file.
			if(temp_file_byte_normal < 128) {out_stream.put(temp_file_byte_normal      );}
			else                            {out_stream.put(temp_file_byte_normal - 256);}
			
			file_size_for_overwriting++;
			one_million_counter++;
			
			//Transforms actual key then establishes a new transformation determinant form new actual key.
			if(one_million_counter == 1000000)
			{	for(int a = 0; a < 100; a++)              //Constructively applies random digits to based on the key (key is used for seeds here.).
				{	srand(transformation_determinant[a]); //WRITES ALTERNATING BETWEEN LEFT TO RIGHT & RIGHT TO LEFT. Alternation is based on the seeds.
					
					if((transformation_determinant[a] % 2) == 0)
					{	for(int b = 0; b < 1000000; b++) //WRITES LEFT TO RIGHT.
						{	temp_integer_arithmetic = actual_key[b];
							temp_integer_arithmetic += (rand() % 256);
							actual_key[b] = (temp_integer_arithmetic % 256);
						}
					}
					else
					{	for(int b = 999999; b >= 0; b--) //WRITES RIGHT TO LEFT.
						{	temp_integer_arithmetic = actual_key[b];
							temp_integer_arithmetic += (rand() % 256);
							actual_key[b] = (temp_integer_arithmetic % 256);
						}
					}
				}
				
				//Adding additional randomness in actual key.
				seeds_sum = 0;
				for(int a = 0; a < 100; a++) {seeds_sum += transformation_determinant[a];}
				srand(seeds_sum); //A new seed comes from the sum of ALL items in the transformation determinant.
				for(int a = 0; a < 1000000; a++) //WRITES LEFT TO RIGHT.
				{	temp_integer_arithmetic = actual_key[a];
					temp_integer_arithmetic += (rand() % 256);
					actual_key[a] = (temp_integer_arithmetic % 256);
				}
				
				//Again, adding additional randomness in actual key.
				seeds_sum = 0;
				for(int a = 0; a < 100; a += 2) {seeds_sum += transformation_determinant[a];}
				srand(seeds_sum); //A new seed comes from the sum of EVERY OTHER item in the transformation determinant.
				for(int a = 999999; a >= 0; a--) //WRITES LEFT TO RIGHT.
				{	temp_integer_arithmetic = actual_key[a];
					temp_integer_arithmetic += (rand() % 256);
					actual_key[a] = (temp_integer_arithmetic % 256);
				}
				
				//Constructs a new transformation determinant using the new actual key.
				actual_key_hopping_read_bookmark = 0;
				for(int a = 0; a < 100; a++)
				{	transformation_determinant[a] = actual_key[actual_key_hopping_read_bookmark];
					actual_key_hopping_read_bookmark += 10000;
				}
				
				one_million_counter = 0; //Resetting write-chunk (every key and its transformation from there is 1MB.)
			}
			
			in_stream.get(temp_file_byte);
		}
		
		in_stream.close();
		out_stream.close();
		
		cout << "\nDone! Plainfile now resides in the given directory.";
	}
	
	
	
	
	
	//Overwriting things (some are partially effective--printed/copied text is cloned from RAM to RAM.)
	//Overwriting the eof file tester.
	sniffed_one_file_character = '\0'; sniffed_one_file_character = -1;            //Binary: 00000000, 111111111.
	
	//Overwriting integer variable path_to_file_null_bookmark (can be used as forensic indicator.)
	path_to_file_null_bookmark = 0; path_to_file_null_bookmark = -2147483648; path_to_file_null_bookmark = 2147483647;
	
	//Overwrites RAM of array key[].
	for(int a = 0; a < 1000001; a++) {key[a] = '\0'; key[a] = -1;}                 //Binary: 00000000, 111111111.
	
	//Overwrites RAM of unsigned array actual_key[].
	for(int a = 0; a < 1000000; a++) {actual_key[a] = '\0'; actual_key[a] = 255;}  //Binary: 00000000, 111111111.
	
	//Overwriting unsigned integer variable seeds_sum.
	seeds_sum = 0; seeds_sum = 4294967295;
	
	//Overwriting RAM of unsigned array transformation_determinant[].              //Binary: 00000000, 111111111.
	for(int a = 0; a < 100; a++) {transformation_determinant[a] = '\0'; transformation_determinant[a] = 255;}
	
	//Asks to overwrite the raw file after encryption.
	if(user_option == 1)
	{	cout << "\n\nOverwrite raw file? y/n: ";
		char wait; cin >> wait;
		
		if(wait == 'y')
		{	out_stream.open(path_to_file_copy_if_overwriting_raw);
			for(int a = 0; a < file_size_for_overwriting; a++) {out_stream << '\0';} out_stream.close(); //Binary: 00000000.
			out_stream.open(path_to_file_copy_if_overwriting_raw);
			for(int a = 0; a < file_size_for_overwriting; a++) {out_stream.put(-1);} out_stream.close(); //Binary: 11111111.
			
			cout << "\nOverwrite finished.\n";
		}
		else {cout << "\nFile unharmed.\n";}
	}
	
	//Asks to overwrite the raw file after decryption, giving the user a moment to observe or transfer it first.
	if(user_option == 2)
	{	cout << "\n\nNow is your chance to observe or copy the plainfile\n"
		     << "onto external devices so that it can be heavily\n"
		     << "overwritten here. Continue? y/n: ";
		char wait; cin >> wait;
		
		if(wait == 'y') //Note the difference here--original path to file.
		{	out_stream.open(path_to_file);
			for(int a = 0; a < file_size_for_overwriting; a++) {out_stream << '\0';} out_stream.close(); //Binary: 00000000.
			out_stream.open(path_to_file);
			for(int a = 0; a < file_size_for_overwriting; a++) {out_stream.put(-1);} out_stream.close(); //Binary: 11111111.
			
			cout << "\nOverwrite finished.\n";
		}
		else {cout << "\nFile unharmed.\n";}
	}
	
	//Overwriting variable file_size_for_overwriting.
	file_size_for_overwriting = 0; file_size_for_overwriting = -9223372036854775807; file_size_for_overwriting = 9223372036854775807;
	
	//Overwriting RAM of array path_to_file[].
	for(int a = 0; a < 10000; a++) {path_to_file[a] = '\0'; path_to_file[a] = -1;} //Binary: 00000000, 111111111.
	
	//Overwriting RAM of array path_to_file_copy_if_overwriting_raw[].             //Binary: 00000000, 111111111.
	for(int a = 0; a < 10000; a++) {path_to_file_copy_if_overwriting_raw[a] = '\0'; path_to_file_copy_if_overwriting_raw[a] = -1;}
}
