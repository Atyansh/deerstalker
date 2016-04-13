#ifndef _CONFIG_SETTINGS_H_
#define _CONFIG_SETTINGS_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <hash_map>
#include <iterator>

namespace util {

	using namespace std;

	const string str_settings_file = "Config.ini";
	const string str_template_settings_file = "ConfigTemplate.ini";

	class ConfigSettings {

	public:
		static string str_port_number;
		static string str_screen_width;
		static string str_screen_height;
		static string str_full_screen;
		static string str_host_name;
		static string str_required_players;

	public:
		static ConfigSettings * config; // Use this static reference instead of your own call to the constructor

		bool checkIfLoaded();
		void saveSettingsFile();
		bool loadSettingsFile();
		bool reloadSettingsFile();

		void copySettingsTemplate();
		void copyMissingSettings();

		bool getValue(string key, bool & ret);
		bool getValue(string key, int & ret);
		bool getValue(string key, float & ret);
		bool getValue(string key, double & ret);
		bool getValue(string key, string & ret);

		void updateValue(string key, bool value);
		void updateValue(string key, int value);
		void updateValue(string key, float value);
		void updateValue(string key, double value);
		void updateValue(string key, string value);

	private:
		//default parameters for a default constructor option
		ConfigSettings(string file_name = str_settings_file, string template_file_name = str_template_settings_file);


		hash_map<string, string> settings;
		bool settings_loaded;

		string file_name;
		string template_file_name;
	};
}

#endif