//Dereck Stewart II
//Lab 1: Music File Information
//Prof. Dr. Plank
//08/31/2020

using namespace std;
#include <iostream>
#include <cstdio>
#include <map>
#include <set>
#include <fstream>
#include <sstream>


// Music File Classes
class Song {
	public:
		Song(string title) : title(title) {};	
		int song_time;

		string title;
};

class Album {
	public:
		Album(string title) : title(title), num_songs(0) {};
		void add_song(string title, int track, int song_time);
		string get_album_time();

		map <int, Song*> Songs;
		string title;
		int num_songs;
};

class Artist {
	public:
		Artist(string name) : name(name), num_songs(1) {};		
		void add_album(string title);
		string get_artist_time();

		map <string, Album*> Albums;
		string name;
		int num_songs;
};

//////////

// Helper Functions
void format_spaces(string &s){
	for (unsigned int i = 0; i < s.size(); i++){
		if (s[i] == '_'){
			s[i] = ' ';
		}
	}

}

int time_to_sec(string time){
	istringstream parse;
	
	int min, sec;

	for (unsigned int i = 0; i < time.size(); i++){
		if (time[i] == ':'){
			time[i] = ' ';
			break;
		}
	}	

	parse.str(time);
	parse >> min >> sec;
	
	return min * 60 + sec;
}

string sec_to_time(int time){
	int min, sec;
	char s_zero = '0';
	string s_time;

	ostringstream sout;

	min = time / 60;
	sec = time % 60;

	if (sec < 10){
		sout << min << ":" << s_zero << sec;
	}
	else {
		sout << min << ":" << sec;
	}

	s_time = sout.str();

	return s_time;
}
//////////

int main(int argc, char** argv){

// argv, argc check
	if (argc != 2) return -1;

// variables
	string file;
	string file_line;
	string title, time, artist, album, genre, track;
	
	map <string, Artist*> Artists;
	map <string, Artist*>::iterator artist_mip;
	
	int song_time;
	int int_track;

	istringstream sin, conv;

	map <string, Album*>::iterator a_mip; // iterator for 'Albums' map
	map <int, Song*>::iterator s_mip; // iterator for 'Songs' map

// Get file
	file = argv[1];
	ifstream fin;
	fin.open(file.c_str());

	if (!fin.is_open()) return -1;

// Parse data
	while (getline(fin, file_line)){
		sin.str("");
		sin.clear();

		sin.str(file_line);

		sin >> title >> time >> artist >> album >> genre >> track;
		
		conv.str("");
		conv.clear();
		
		conv.str(track);
		conv >>	int_track;	

		format_spaces(title);
		format_spaces(album);
		format_spaces(artist);
		song_time = time_to_sec(time);
	
	// Build Objects
		artist_mip = Artists.find(artist);
		if (artist_mip == Artists.end()){	// if artist is not in map
			Artist* new_artist = new Artist(artist);	// create new artist

			new_artist->add_album(album);		// add new album to artist
			a_mip = new_artist->Albums.find(album);
			
			a_mip->second->add_song(title, int_track, song_time);	// add new song to album

			Artists.insert(make_pair(artist, new_artist));	// add artist to 'Artists' map
			
			continue;
		}
		
		a_mip = artist_mip->second->Albums.find(album);
		
		if (a_mip == artist_mip->second->Albums.end()){	// if album is not in map	
			artist_mip->second->add_album(album); // add new album to artist 

			a_mip = artist_mip->second->Albums.find(album);
			a_mip->second->add_song(title, int_track, song_time);	// add new song

			artist_mip->second->num_songs++;
	
		} else{		// if just a new song
			a_mip->second->add_song(title, int_track, song_time);	// add new song

			artist_mip->second->num_songs++;
		}	

	}

// Formatted output
	string spaces = "        ";
	string spaces_1 = "                ";

	for (artist_mip = Artists.begin(); artist_mip != Artists.end(); artist_mip++){
		printf("%s: %d, %s\n", artist_mip->first.c_str(), artist_mip->second->num_songs, artist_mip->second->get_artist_time().c_str()); 

		for (a_mip = artist_mip->second->Albums.begin(); a_mip != artist_mip->second->Albums.end(); a_mip++){
			printf("%s%s: %d, %s\n", spaces.c_str(), a_mip->first.c_str(), a_mip->second->num_songs, a_mip->second->get_album_time().c_str());					
	
			for (s_mip = a_mip->second->Songs.begin(); s_mip != a_mip->second->Songs.end(); s_mip++){
				printf("%s%d. %s: %s\n", spaces_1.c_str(), s_mip->first, s_mip->second->title.c_str(), sec_to_time(s_mip->second->song_time).c_str());		
			}	
		}	
	 }

	fin.close();
	return 0;
}

void Artist::add_album(string title){
	Album* new_album = new Album(title);
	this->Albums.insert(make_pair(title, new_album));
}

string Artist::get_artist_time(){
	string s_time;
	int time = 0;
	map <string, Album*>::iterator mip;
	
	for (mip = this->Albums.begin(); mip != this->Albums.end(); mip++){
			s_time = mip->second->get_album_time();
			time += time_to_sec(s_time);
	}

	return sec_to_time(time);
}


void Album::add_song(string title, int track, int song_time){
	Song* new_song = new Song(title);
	new_song->song_time = song_time;

	this->Songs.insert(make_pair(track, new_song));
	this->num_songs++;
}

string Album::get_album_time(){
	string s_time;
	int sec = 0;

	map <int, Song*>::iterator s_mip;

	for (s_mip = this->Songs.begin(); s_mip != this->Songs.end(); s_mip++){	
		sec += s_mip->second->song_time;
	}

	return sec_to_time(sec);
}
