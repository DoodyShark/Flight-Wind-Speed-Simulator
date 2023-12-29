#include <iostream> // To display messages into the output stream
#include <fstream> // To read and write into files
#include <ctime> // To seed the random function
#include <iomanip> // To organize the data in tabular form
#define ROW1 4 // Length of the windConfig array
#define ROW2_3 5 // Length of the stormConfig and burstConfig arrays
#define W 20 // Width to be used in the setw() function
#define DELTA 0.000001 // To account for precision in double/float variables

using namespace std;

// Function prototypes
int configure(double windConfig[], double stormConfig[], double burstConfig[]);

void generateSpeedData(const double windConfig[]);

void generateStormData(const double stormConfig[], double duration, double step);

void generateBurstData(const double info[], double duration, double step);

void generateSimulationData(double duration, double step);

int main() {
	srand(time(0)); // Seeding the random function
	// Creating arrays for storing configuration data
	double windConfig[ROW1] = { 0.0 };
	double stormConfig[ROW2_3] = { 0.0 };
	double burstConfig[ROW2_3] = { 0.0 };
	int errorCount = configure(windConfig, stormConfig, burstConfig); // Configuring and validating data
	if (errorCount > 0) {
		cerr << errorCount << " errors encountered in simulationConfiguration.txt . . . " << endl;
		cerr << "Please perform necessary edits and rerun the program . . ." << endl;
		exit(errorCount);
	}
	cout << "Configuration performed successfully . . ." << endl;
	// Create and fill WindSpeedData.txt
	generateSpeedData(windConfig);
	cout << "WindSpeedData.txt generated successfully . . ." << endl;
	// Create and fill StormData.txt
	generateStormData(stormConfig, windConfig[2], windConfig[3]);
	cout << "StormData.txt generated successfully . . ." << endl;
	// Create and fill BurstData.txt
	generateBurstData(burstConfig, windConfig[2], windConfig[3]);
	cout << "BurstData.txt generated successfully . . ." << endl;
	// Create and fill WindSimulationData.txt
	generateSimulationData(windConfig[2], windConfig[3]);
	cout << "WindSimulationData.txt generated successfully . . ." << endl;
	cout << endl << "-----------------------------------------------------------" << endl;
	cout << "Thanks for using our services . . ." << endl;
	return(0);
}

// The configure function reads all the user-specified constraints and values from the file simulationConfiguration.txt and validates said data
int configure(double windConfig[], double stormConfig[], double burstConfig[]) {
	int errorCount(0);
	ifstream fileReader;
	fileReader.open("simulationConfiguration.txt", ios::in);
	if (fileReader.fail()) {
		cerr << "ERROR: Failed to open file . . ." << endl;
		exit(-1);
	}
	// Reading wind speed configuration data
	for (int i = 0; i < ROW1; i++) {
		fileReader >> windConfig[i];
	}
	// Reading storm configuration data
	for (int i = 0; i < ROW2_3; i++) {
		fileReader >> stormConfig[i];
	}
	// Reading burst configuration data
	for (int i = 0; i < ROW2_3; i++) {
		fileReader >> burstConfig[i];
	}
	// Validating all configuration data based on constraints mentioned in the lab report
	if (windConfig[0] < windConfig[1] || windConfig[0] < 0) {
		cerr << "Invalid input: Wind speed has to be non-negative and less than the gust speed . . ." << endl;
		errorCount++;
	}
	if (windConfig[1] < 0) {
		cerr << "Invalid input: Gust speed has to be non-negative . . ." << endl;
		errorCount++;
	}
	if (windConfig[2] <= 0) {
		cerr << "Invalid input: Time must be positive . . ." << endl;
		errorCount++;
	}
	if (windConfig[3] <= 0) {
		cerr << "Invalid input: Step-size must be positive . . ." << endl;
		errorCount++;
	}
	if (stormConfig[0] < 0) {
		cerr << "Invalid input: Probability must be non-negative . . ." << endl;
		errorCount++;
	}
	if (stormConfig[1] <= 0) {
		cerr << "Invalid input: Min storm amplitude must be positive . . ." << endl;
		errorCount++;
	}
	if (stormConfig[2] <= 0 || stormConfig[2] < stormConfig[1]) {
		cerr << "Invalid input: Maximum amplitude must be positive and greater than the minimum amplitude . . ." << endl;
		errorCount++;
	}
	if (stormConfig[3] <= 0) {
		cerr << "Invalid input: Min storm duration must be positive . . ." << endl;
		errorCount++;
	}
	if (stormConfig[4] <= 0 || stormConfig[4] < stormConfig[3]) {
		cerr << "Invalid input: Max storm duration must be positive and greater than the minimum duration . . ." << endl;
		errorCount++;
	}
	if (burstConfig[0] < 0) {
		cerr << "Invalid input: Probability must be non-negative . . ." << endl;
		errorCount++;
	}
	if (burstConfig[1] <= 0) {
		cerr << "Invalid input: Min storm amplitude must be positive . . ." << endl;
		errorCount++;
	}
	if (burstConfig[2] <= 0 || burstConfig[2] < burstConfig[1]) {
		cerr << "Invalid input: Maximum amplitude must be positive and greater than the minimum amplitude . . ." << endl;
		errorCount++;
	}
	if (burstConfig[3] <= 0) {
		cerr << "Invalid input: Min storm duration must be positive . . ." << endl;
		errorCount++;
	}
	if (burstConfig[4] <= 0 || burstConfig[4] < burstConfig[3]) {
		cerr << "Invalid input: Max strom duration must be positive and greater than the minimum duration . . ." << endl;
		errorCount++;
	}
	fileReader.close();
	return errorCount;
}

// The generateSpeedData function creates the WindSpeedData.txt file which stores a random value of speed within the user-specified constraints for each time in the duration
void generateSpeedData(const double windConfig[]) {
	ofstream writer("WindSpeedData.txt", ios::trunc);
	if (writer.fail()) {
		cerr << "ERROR: Failed to open file . . ." << endl;
		exit(-1);
	}
	// Prepare column headers
	writer << setw(W) << left << "Time (s)" << setw(W) << "Wind-Speed" << endl;
	for (double t = 0; t <= (windConfig[2]) + DELTA; t += windConfig[3]) {
		writer << left << setw(W) << t << setw(W) << (windConfig[0] + ((double)rand() / RAND_MAX - 0.5) * 2 * windConfig[1]) /*Random value between -windConfig[1] and +windConfig[1] (-gust and +gust)*/ << endl;
	}
	writer.close();
}
void generateStormData(const double stormConfig[], double duration, double step) {
	ofstream writer("StormData.txt", ios::trunc);
	if (writer.fail()) {
		cerr << "ERROR: Failed to open file . . ." << endl;
		exit(-1);
	}
	// Prepare column headers
	writer << setw(W) << left << "Time (s)" << setw(W) << "Storm-Magnitude" << endl;
	for (double t = 0; t <= duration + DELTA; t+=step) {
		if ((double)rand() / RAND_MAX < stormConfig[0] || stormConfig[0] ==1) { // Generates a storm based on the specified probability from the configuration file
			double T = (((double)rand() / RAND_MAX) * (stormConfig[4] - stormConfig[3]) + stormConfig[3]); // Random value between stormConfig[3] and stormConfig[4] (Min and max duration)
			double initialT = t;
			for (t = initialT; t <= T + initialT + DELTA && t <= duration + DELTA; t+=step) {
				writer << left << setw(W) << t << setw(W) << (((double)rand() / RAND_MAX) * (stormConfig[2] - stormConfig[1]) + stormConfig[1]) /*Random value between stormConfig[1] and stormConfig[2] (Min and max amplitude)*/ << endl;
			}
			t-=step; // Before the loop exits, it will increment to the next t value. This is to counteract this incrementation.
		}
		else
			writer << left << setw(W) << t << setw(W) << 0 << endl;
	}
	writer.close();
}

void generateBurstData(const double burstConfig[], double duration, double step) {
	ofstream writer("BurstData.txt", ios::trunc);
	if (writer.fail()) {
		cerr << "ERROR: Failed to open file . . ." << endl;
		exit(-1);
	}
	ifstream reader("StormData.txt", ios::ate); // The file needs to be opened in the ios::ate mode to allow for skipping the column headers
	if (reader.fail()) {
		cerr << "ERROR: Failed to open file . . ." << endl;
		exit(-1);
	}
	reader.seekg(2*W, ios::beg); // Places cursor after the column headers to begin reading the double values
	writer << setw(W) << left << "Time (s)" << setw(W) << "Burst-Magnitude" << endl;
	double x(0), s(0); // x is a rubbish variable that will be use to skip through the unwanted data
	reader >> x >> s;
	for (double t = 0; t <= duration + DELTA; t += step) {
		if (!(s <DELTA && s>-DELTA)) { // Equivalent to s != 0 but accounts for uncertainty of double values. Checks if there is a storm, and if so, a microburst can occur
			if ((double)rand() / RAND_MAX < burstConfig[0] || burstConfig[0] == 1) { // Generates a microburst according to the probability defined in the configuration file
				double T = (((double)rand() / RAND_MAX) * (burstConfig[4] - burstConfig[3]) + burstConfig[3]); // Random value between burstConfig[3] and burstConfig[4] (Min and max duration)
				double initialT = t;
				for (t = initialT; t <= T + initialT + DELTA && t <= duration + DELTA && !(s<DELTA && s>-DELTA) /*Equivalent to s != 0 but accounts for uncertainty of double values*/; t += step) {
					writer << left << setw(W) << t << setw(W) << (((double)rand() / RAND_MAX) * (burstConfig[2] - burstConfig[1]) + burstConfig[1])  /*Random value between burstConfig[1] and burstConfig[2] (Min and max amplitude)*/  << endl;
					reader >> x >> s;
				}
				t -= step; // Before the loop exits, it will increment to the next t value. This is to counteract this incrementation.
			}
			else {
				writer << setw(W) << left << t << setw(W) << 0 << endl;
				reader >> x >> s;
			}
		}
		else {
			writer << setw(W) << left << t << setw(W) << 0 << endl;
			reader >> x >> s;
		}
	}
	writer.close();
	reader.close();
}

void generateSimulationData(double duration, double step) {
	ofstream writer("WindSimulationData.txt", ios::trunc);
	// The files need to be opened in the ios::ate mode to allow for skipping the column headers
	ifstream wind("WindSpeedData.txt", ios::ate);
	ifstream storm("StormData.txt", ios::ate);
	ifstream burst("BurstData.txt", ios::ate);
	if (wind.fail() || storm.fail() || burst.fail() || writer.fail()) {
		cerr << "ERROR: Failed to open file . . ." << endl;
		exit(-1);
	}
	double x(0), windS(0), stormM(0), burstM(0); // x is a rubbish variable that will be used to store unneeded data
	bool pres(false);
	// Move all cursors in front of the column headers to begin reading the doubles
	wind.seekg(2 * W, ios::beg);
	storm.seekg(2 * W, ios::beg);
	burst.seekg(2 * W, ios::beg);
	writer << setw(W) << left << "Time (s)" << setw(W) << "Speed" << setw(W) << "Storm present?" << endl;
	for (double t = 0; t <= duration + DELTA; t+=step) {
		wind >> x >> windS;
		storm >> x >> stormM;
		burst >> x >> burstM;
		if (!(stormM > -DELTA && stormM < DELTA)) // checks if a storm is present which will be printed into the WindSimulation.txt file
			pres = true; 
		else
			pres = false;
		writer << setw(W) << left << t << setw(W) << (windS + stormM + burstM) << setw(W) << pres << endl;
	}
	writer.close();
	wind.close();
	storm.close();
	burst.close();
}