// Mini project: Learning Amazon S3 and C++ skills
// This script demonstrates creating, managing, and uploading files to an S3 bucket securely using AWS CLI.

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>

// Function to execute system commands and handle errors
std::string executeCommand(const std::string &command) {
    char buffer[128];
    std::string result;
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("Failed to execute command: " + command);
    }
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

// Function to create an S3 bucket
void createBucket(const std::string &bucketName) {
    std::string command = "aws s3 mb s3://" + bucketName;
    std::cout << "Creating bucket: " << bucketName << std::endl;
    std::cout << executeCommand(command) << std::endl;
}

// Function to configure bucket policy
void configureBucketPolicy(const std::string &bucketName, const std::string &policyFile) {
    std::string command = "aws s3api put-bucket-policy --bucket " + bucketName + " --policy file://" + policyFile;
    std::cout << "Configuring bucket policy for: " << bucketName << std::endl;
    std::cout << executeCommand(command) << std::endl;
}

// Function to securely upload a file to an S3 bucket
void uploadFile(const std::string &bucketName, const std::string &filePath) {
    std::string command = "aws s3 cp " + filePath + " s3://" + bucketName;
    std::cout << "Uploading file to bucket: " << bucketName << std::endl;
    std::cout << executeCommand(command) << std::endl;
}

// Function to validate file existence
bool fileExists(const std::string &fileName) {
    std::ifstream file(fileName);
    return file.good();
}

int main() {
    try {
        // Define bucket name, policy file, and file to upload
        const std::string bucketName = "my-mini-project-bucket";
        const std::string policyFile = "bucket-policy.json";
        const std::string fileToUpload = "test-file.txt";

        // Create a sample file for upload
        std::ofstream outFile(fileToUpload);
        if (outFile.is_open()) {
            outFile << "This is a test file for S3 upload demonstration.";
            outFile.close();
        } else {
            throw std::runtime_error("Failed to create test file: " + fileToUpload);
        }

        // Validate file creation
        if (!fileExists(fileToUpload)) {
            throw std::runtime_error("File does not exist: " + fileToUpload);
        }

        // Create S3 bucket
        createBucket(bucketName);

        // Ensure the policy file exists before configuring bucket policy
        if (fileExists(policyFile)) {
            configureBucketPolicy(bucketName, policyFile);
        } else {
            std::cerr << "Policy file not found. Skipping policy configuration." << std::endl;
        }

        // Upload file to the S3 bucket
        uploadFile(bucketName, fileToUpload);

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
