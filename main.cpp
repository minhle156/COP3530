// Minh Le

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <utility>
#include <chrono>
using namespace std;
using namespace std::chrono;

struct Cat {
    int id;
    string url;
    string age;
    string gender;
    string size;
    string coat;
    string breed;
    string image;
};

Cat readOneLine(istream& is) {
    string line;
    getline(is, line);

    if (!is) {
        return Cat();
    }

    int comma = line.find(",");

    string indexStr = line.substr(0, comma);
    int index = stoi(indexStr);
    line = line.substr(comma + 1);

    comma = line.find(",");
    string idString = line.substr(0, comma);
    int id = stoi(idString);
    line = line.substr(comma + 1);

    comma = line.find(",");
    string url = line.substr(0, comma);
    line = line.substr(comma + 1);

    int start = line.find("Cat,") + 4;
    int end = line.find(",", start + 1);
    string age = line.substr(start, end - start);
    line = line.substr(end + 1);

    comma = line.find(",");
    string gender = line.substr(0, comma);
    line = line.substr(comma + 1);

    comma = line.find(",");
    string size = line.substr(0, comma);
    line = line.substr(comma + 1);

    comma = line.find(",");
    string coat = line.substr(0, comma);
    line = line.substr(comma + 1);

    comma = line.find(",");
    string breed = line.substr(0, comma);
    line = line.substr(comma + 1);

    start = line.find("'full':") + 9;
    end = line.find("'},", start + 1);
    string image = line.substr(start, end - start);

    Cat cat;
    cat.id = id;
    cat.url = url;
    cat.age = age;
    cat.gender = gender;
    cat.size = size;
    cat.coat = coat;
    cat.breed = breed;
    cat.image = image;

    return cat;
}

vector<Cat> readDataFile(istream& is) {
    string header;
    getline(is, header);
    vector<Cat> cats;

    cout << "Start reading " << endl;
    while(is) {
        Cat cat = readOneLine(is);
        if(is) {
            cats.push_back(cat);
        }
    }
    cout << "Input " << cats.size() << " cats" << endl;
    return cats;
}

bool compareLess(Cat a, Cat b, string sortField) {
	if (sortField == "id") {
		return a.id < b.id;
	}
	if (sortField == "url") {
		return a.url < b.url;
	}
	if (sortField == "age") {
		return a.age < b.age;
	}
	if (sortField == "gender") {
		return a.gender < b.gender;
	}
	if (sortField == "size") {
		return a.size < b.size;
	}
	if (sortField == "coat") {
		return a.coat < b.coat;
	}
	if (sortField == "breed") {
		return a.breed < b.breed;
	}
	if (sortField == "image") {
		return a.image < b.image;
	}
	cout << "Invalid field " << sortField << endl;
	exit(1);
}

bool compareLessEQ(Cat a, Cat b, string sortField) {
	if (sortField == "id") {
			return a.id <= b.id;
		}
		if (sortField == "url") {
			return a.url <= b.url;
		}
		if (sortField == "age") {
			return a.age <= b.age;
		}
		if (sortField == "gender") {
			return a.gender <= b.gender;
		}
		if (sortField == "size") {
			return a.size <= b.size;
		}
		if (sortField == "coat") {
			return a.coat <= b.coat;
		}
		if (sortField == "breed") {
			return a.breed <= b.breed;
		}
		if (sortField == "image") {
			return a.image <= b.image;
		}
		cout << "Invalid field " << sortField << endl;
		exit(1);
}

void mergeData(vector<Cat>& dataset, int start, int middle, int end, string sortField) {
    // Merge sub arrays
    int n1 = middle - start + 1;
    int n2 = end - middle;
    vector<Cat> X, Y;

    for (int i = 0; i < n1; i++) {
        X.push_back(dataset.at(start + i));
    }
    for (int j = 0; j < n2; j++) {
        Y.push_back(dataset.at(middle + 1 + j));
    }

    int i, j, k;
    i = 0; // X[i]
    j = 0; // Y[j]
    k = start; // dataset[k]

    while ((i < n1) && (j < n2)) {
        if (compareLess(X.at(i), Y.at(j), sortField)) {
            dataset.at(k) = X.at(i);
            i++;
        }
        else {
            dataset.at(k) = Y.at(j);
            j++;
        }
        k++;
    }

    while (i < n1) {
        dataset.at(k) = X.at(i);
        i++;
        k++;
    }

    while (j < n2) {
        dataset.at(k) = Y.at(j);
        j++;
        k++;
    }
}

int partitionData(vector<Cat>& dataset, int low, int high, string sortField) {
// Select the pivot element
	Cat pivot = dataset[low];
	int up = low, down = high;
	while (up < down) {
		for (int j = up; j < high; j++) {
			if (!compareLessEQ(dataset[up], pivot, sortField))
				break;
			up++;
		}
		for (int j = high; j > low; j--) {
			if (compareLess(dataset[down], pivot, sortField))
				break;
			down--;
		}
		if (up < down)
			swap(dataset[up], dataset[down]);
	}
	swap(dataset[low], dataset[down]);
	return down;
}

void quickSortData(vector<Cat>& dataset, int low, int high, string sortField) {
	if (low < high) {
		int pivot = partitionData(dataset, low, high, sortField);
		quickSortData(dataset, low, pivot - 1, sortField);
		quickSortData(dataset, pivot + 1, high, sortField);
	}
}

void mergeSortData(vector<Cat>& dataset, int start, int end, string sortField) {

    if (start < end) {
        int middle = (start + end) / 2;
        mergeSortData(dataset, start, middle, sortField);
        mergeSortData(dataset, middle + 1, end, sortField);

        mergeData(dataset, start, middle, end, sortField);
    }
}

void checkOrder(vector<Cat>& dataset, string sortField) {
	for (int i = 0; i < dataset.size() - 2; i++) {
		if (!compareLessEQ(dataset.at(i), dataset.at(i + 1), sortField)) {
			cout << "Incorrect order by ID " << i << endl;
			return;
		}
	}
	cout << "Data sorted" << endl;
}

bool match(Cat cat, string field, string value) {
	if (field == "age") {
		return (cat.age.find(value) != string::npos);
	}
	if (field == "url") {
		return (cat.url.find(value) != string::npos);
	}
	if (field == "gender") {
		return (cat.gender.find(value) != string::npos);
	}
	if (field == "size") {
		return (cat.size.find(value) != string::npos);
	}
	if (field == "coat") {
		return (cat.coat.find(value) != string::npos);
	}
	if (field == "breed") {
		return (cat.breed.find(value) != string::npos);
	}
	if (field == "image") {
		return (cat.image.find(value) != string::npos);
	}
	cout << "Invalid search field " << value << endl;
	exit(1);
}

vector<Cat> filterData(vector<Cat>& dataset, string field, string value) {
	vector<Cat> output;

	for (Cat cat : dataset) {
		if (match(cat, field, value)) {
			output.push_back(cat);
		}
	}

	return output;
}

void printData(vector<Cat>& dataset, string outputFilename) {
	cout << "Results " << dataset.size() << " cats" << endl;
	ofstream out(outputFilename);
	for (Cat cat : dataset) {
		out << "<img width=600 height=400 src=\"" << cat.image << "\"><br>" << endl;
	}
}


int main(int argc, char* argv[]) {
	if (argc != 5) {
		cout << "Usage: main inputfile.csv searchField searchValue sortField" << endl;
		cout << "       * searchField: url, age, gender, size, coat, breed, image" << endl;
		cout << "       * sortField: id, url, age, gender, size, coat, breed, image" << endl;
	}
	string inputfile = argv[1];
	string searchField = argv[2];
	string searchValue = argv[3];
	string sortField = argv[4];
    ifstream is(inputfile);
    vector<Cat> dataset = readDataFile(is);

    vector<Cat> dataset1 = filterData(dataset, searchField, searchValue);
    auto start1 = high_resolution_clock::now();
    mergeSortData(dataset1, 0, dataset1.size() - 1, sortField);
    auto stop1 = high_resolution_clock::now();
    cout << "mergesort time " << duration_cast<milliseconds>(stop1 - start1).count() << endl;
    printData(dataset1, "mergesort.html");
    checkOrder(dataset1, sortField);

    vector<Cat> dataset2 = filterData(dataset, searchField, searchValue);
    auto start2 = high_resolution_clock::now();
	quickSortData(dataset2, 0, dataset1.size() - 1, sortField);
	auto stop2 = high_resolution_clock::now();
	cout << "quicksort time " << duration_cast<milliseconds>(stop2 - start2).count() << endl;
	printData(dataset2, "quicksort.html");
	checkOrder(dataset2, sortField);

	system("open mergesort.html quicksort.html");

    return 0;
}
