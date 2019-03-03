#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <atomic>
#include <boost/lexical_cast.hpp>

using boost::lexical_cast;
using boost::bad_lexical_cast;

inline std::chrono::high_resolution_clock::time_point get_current_time_fenced()
{
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template<class D>
inline long long to_us(const D& d)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

int read_data_from_file(const std::string& input_file_name, std::ifstream &input_stream)
{
    input_stream.open(input_file_name);

    if (!input_stream || !input_stream.is_open())
    {
        std::cout << "Error. File is empty or cannot be opened" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void stream_to_string(std::ifstream &input_stream, std::vector<std::string> &input_data)
{
    std::stringstream local_input_stream;

    //save cursor position in input file stream
    std::streampos p = input_stream.tellg();

    //copy input file stream to local string stream to prevent clearing of file stream
    local_input_stream << input_stream.rdbuf();

    //return cursor to the previous position
    input_stream.seekg(p);

    //move data from stream to vector of strings
    std::string line;
    while (getline(local_input_stream, line))
    {
        input_data.push_back(line);
    }
}

auto convert_with_stream(std::ifstream &input_stream, std::vector<double> &numbers)
{
    auto stage_start_time = get_current_time_fenced();

    double i;
    while (input_stream >> i) { numbers.push_back(i); }

    auto finish_time = get_current_time_fenced();
    return finish_time - stage_start_time;
}

auto convert_with_stod(const std::vector<std::string> &input_data, std::vector<double> &numbers)
{
    auto stage_start_time = get_current_time_fenced();

    for (size_t i = 0; i < input_data.size(); ++i)
    {
        numbers.push_back(std::stod(input_data[i]));
    }

    auto finish_time = get_current_time_fenced();
    return finish_time - stage_start_time;
}

auto convert_with_c_style(const std::vector<std::string> &input_data, std::vector<double> &numbers)
{
    auto stage_start_time = get_current_time_fenced();

    for (size_t i = 0; i < input_data.size(); ++i)
    {
        double n;
        sscanf(input_data[i].c_str(), "%lf", &n);
        numbers.push_back(n);
    }

    auto finish_time = get_current_time_fenced();
    return finish_time - stage_start_time;
}

auto convert_with_boost(const std::vector<std::string> &input_data, std::vector<double> &numbers)
{
    auto stage_start_time = get_current_time_fenced();

    for (size_t i = 0; i < input_data.size(); ++i)
    {
        try
        {
            numbers.push_back(lexical_cast<double>(input_data[i]));
        } catch (bad_lexical_cast &)
        {
            numbers.push_back(0);
        }
    }

    auto finish_time = get_current_time_fenced();
    return finish_time - stage_start_time;
}

enum FuncType
{
    Zero, UseStream, UseStod, UseCStyle, UseBoost
};

void compute_result(double result[], const std::vector<double> &numbers)
{
    //calculate sum of elements
    for (double n : numbers)
    {
        result[0] += n;
    }
    //calculate average
    result[1] = result[0] / (double) numbers.size();
}

int write_result_to_file(const std::string &output_file_name, const double result[])
{
    std::ofstream result_file(output_file_name);

    if (!result_file.is_open())
    {
        std::cout << "Error occured while writing result to file" << std::endl;
        return EXIT_FAILURE;
    }

    result_file << result[0] << std::endl << result[1] << std::endl;
    result_file.close();
    return EXIT_SUCCESS;
}

int main(int argc, char** argv) {

    const int preferable_argc_value = 4;
    if (argc != preferable_argc_value)
    {
        std::cout << "Error. Incorrect input. Not enough arguments." << std::endl;
        return EXIT_FAILURE;
    }

    int func_type = std::stoi(argv[1]);
    std::string input_file_name = argv[2];
    std::string output_file_name = argv[3];
    input_file_name = "../data/" + input_file_name;
    output_file_name = "../data/" + output_file_name;
    std::ifstream input_stream;

    if (read_data_from_file(input_file_name, input_stream) == EXIT_FAILURE)
        return EXIT_FAILURE;

    //numbers as strings
    std::vector<std::string> input_data;
    //convert input file stream to vaector of strings
    stream_to_string(input_stream, input_data);
    std::vector<double> numbers;
    std::common_type_t<std::chrono::nanoseconds> total_time;

    switch (func_type)
    {
        case UseStream:
            std::cout << "Type: " << UseStream << std::endl;
            total_time = convert_with_stream(input_stream, numbers);
            break;
        case UseStod:
            std::cout << "Type: " << UseStod << std::endl;
            total_time = convert_with_stod(input_data, numbers);
            break;
        case UseCStyle:
            std::cout << "Type: " << UseCStyle << std::endl;
            total_time = convert_with_c_style(input_data, numbers);
            break;
        case UseBoost:
            std::cout << "Type: " << UseBoost << std::endl;
            total_time = convert_with_boost(input_data, numbers);
            break;
        default:
            std::cout << "Error. Incorrect input. Use range 1:4 for function type." << std::endl;
            return EXIT_FAILURE;
    }

    std::cout << "Total time: " << to_us(total_time) << std::endl;

    input_stream.close();

    double result[] = { 0, 0 };
    compute_result(result, numbers);

    if (!write_result_to_file(output_file_name, result))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
