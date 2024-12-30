// Mini project: Learning Amazon S3 and C++ skills
// This script demonstrates creating, managing, and uploading files to an S3 bucket securely using AWS CLI.

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>

using namespace std;

// Function to execute system commands and handle errors
string executeCommand(const string& command) {
    char buffer[128];
    string result;
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        throw runtime_error("Failed to execute command: " + command);
    }
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

// Function to create an S3 bucket
void createBucket(const string& bucketName) {
    string command = "aws s3 mb s3://" + bucketName;
    cout << "Creating bucket: " << bucketName << endl;
    cout << executeCommand(command) << endl;
}

// Function to configure bucket policy
void configureBucketPolicy(const string& bucketName, const string& policyFile) {
    string command = "aws s3api put-bucket-policy --bucket " + bucketName + " --policy file://" + policyFile;
    cout << "Configuring bucket policy for: " << bucketName << endl;
    cout << executeCommand(command) << endl;
}

// Function to securely upload a file to an S3 bucket
void uploadFile(const string& bucketName, const string& filePath) {
    string command = "aws s3 cp " + filePath + " s3://" + bucketName;
    cout << "Uploading file to bucket: " << bucketName << endl;
    cout << executeCommand(command) << endl;
}

// Function to validate file existence
bool fileExists(const string& fileName) {
    ifstream file(fileName);
    return file.good();
}

int main() {
    // try/except - If the file does not exist, the FileNotFoundError is caught and prints
    // a user-friendly message.
    try {
        // Define bucket name, policy file, and file to upload
        const string bucketName = "my-mini-project-bucket";
        const string policyFile = "bucket-policy.json";
        const string fileToUpload = "test-file.txt";

        // Create a sample file for upload
        ofstream outFile(fileToUpload);
        if (outFile.is_open()) {
            outFile << "This is a test file for S3 upload demonstration.";
            outFile.close();
        }
        else {
            throw runtime_error("Failed to create test file: " + fileToUpload);
        }

        // Validate file creation
        if (!fileExists(fileToUpload)) {
            throw runtime_error("File does not exist: " + fileToUpload);
        }

        // Create S3 bucket
        createBucket(bucketName);

        // Ensure the policy file exists before configuring bucket policy
        if (fileExists(policyFile)) {
            configureBucketPolicy(bucketName, policyFile);
        }
        else {
            cerr << "Policy file not found. Skipping policy configuration." << endl;
        }

        // Upload file to the S3 bucket
        uploadFile(bucketName, fileToUpload);

    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}

