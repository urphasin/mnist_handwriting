#include "data_handler.hpp"

data_handler::data_handler() 
{
    data_array = new std::vector<data *>;
    training_data = new std::vector<data *>;
    test_data = new std::vector<data *>;
    validation_data = new std::vector<data *>;
}
data_handler::~data_handler()
{
    // free dynamically allocated stuff
}

void data_handler::read_feature_vector(std::string path)
{
    uint32_t header[4]; // |MAGIC|NUM IMAGES|ROW_SIZE|COLUMN_SIZE
    unsigned char bytes[4];
    FILE *f = fopen(path.c_str(), "r");
    
    if(f)
    {
        for (int i = 0; i < 4; i++) {
            if(fread(bytes, sizeof(bytes), 1, f))
            {
                header[i] = convert_to_little_endian(bytes);
            }
        }
        printf("Done getting Input File Header.\n");
        int image_size = header[2] * header[3];
        for (int i = 0; i < header[1]; i++)
        {
            data *d = new data();
            uint8_t element[1];

            for (int j = 0; j < image_size; j++) {
                if(fread(element, sizeof(element), 1, f)) {
                    (*d).append_to_feature_vector(element[0]);
                } else {
                    printf("Error reading from file.\n");
                    exit(1);
                }
            }
            (*data_array).push_back(d);
        }
        printf("Successfully read and stored %lu feature vector.\n", (*data_array).size());
    }
    else
    {
        printf("Problem getting file.\n");
        exit(1);
    }
}
void data_handler::read_feature_labels(std::string path)
{
    uint32_t header[2]; // |MAGIC|NUM IMAGES|ROW_SIZE|COLUMN_SIZE
    unsigned char bytes[4];
    FILE *f = fopen(path.c_str(), "r");
    
    if(f)
    {
        for (int i = 0; i < 2; i++) {
            if(fread(bytes, sizeof(bytes), 1, f))
            {
                header[i] = convert_to_little_endian(bytes);
            }
        }
        printf("Done getting Label File Header.\n");
        for (int i = 0; i < header[1]; i++)
        {
            uint8_t element[1];

            if(fread(element, sizeof(element), 1, f)) {
                (*(*data_array).at(i)).set_label(element[0]);
            }
            else
            {
                printf("Error reading from file.\n");
                exit(1);
            }
        }
        printf("Successfully read and stored %lu label.\n", (*data_array).size());
    }
    else
    {
        printf("Problem getting file.\n");
        exit(1);
    }
}
void data_handler::split_data()
{
    std::unordered_set<int> used_indexes;
    int train_size = (*data_array).size() * TRAIN_SET_PERCENT;
    int test_size = (*data_array).size() * TEST_SET_PERCENT;
    int validation_size = (*data_array).size() * VALIDATION_SET_PERCENT;


    // TRAINING DATA

    int count = 0;
    while(count < train_size)
    {
        int random_index = rand() % (*data_array).size(); // 0 & data_array size - 1
        if (used_indexes.find(random_index) == used_indexes.end())
        {
            (*training_data).push_back((*data_array).at(random_index));
            used_indexes.insert(random_index);
            count++;
        }
    }

    // TEST DATA

    count = 0;
    while(count < test_size)
    {
        int random_index = rand() % (*data_array).size(); // 0 & data_array size - 1
        if (used_indexes.find(random_index) == used_indexes.end())
        {
            (*test_data).push_back((*data_array).at(random_index));
            used_indexes.insert(random_index);
            count++;
        }
    }

    // VALIDATION DATA
    count = 0;
    while(count < validation_size)
    {
        int random_index = rand() % (*data_array).size(); // 0 & data_array size - 1
        if (used_indexes.find(random_index) == used_indexes.end())
        {
            (*validation_data).push_back((*data_array).at(random_index));
            used_indexes.insert(random_index);
            count++;
        }
    }

    printf("Training Data Size: %lu.\n", training_data->size());
    printf("Test Data Size: %lu.\n", test_data->size());
    printf("Validation Data Size: %lu.\n", validation_data->size());
}
void data_handler::count_classes()
{
    int count = 0;
    for (unsigned int i = 0; i < (*data_array).size(); i++)
    {
        if(class_map.find(data_array->at(i)->get_label()) == class_map.end())
        {
            class_map[(*(*data_array).at(i)).get_label()] = count;
            data_array->at(i)->set_enumerated_label(count);
            count++;
        }
    }
    num_classes = count;
    printf("Successfully Extracted %d Unique Classes.\n", num_classes);
}

uint32_t data_handler::convert_to_little_endian(const unsigned char* bytes)
{
    return ((uint32_t)bytes[0] << 24 | 
            (uint32_t)bytes[1] << 16 | 
            (uint32_t)bytes[2] << 8  | 
            (uint32_t)bytes[3] << 0);
}

std::vector<data *> *data_handler::get_training_data()
{
    return training_data;
}
std::vector<data *> *data_handler::get_test_data()
{
    return test_data;
}
std::vector<data *> *data_handler::get_validation_data()
{
    return validation_data;
}


int main()
{
    data_handler *dh = new data_handler();
    dh->read_feature_vector("../File/train-images.idx3-ubyte");
    dh->read_feature_labels("../File/train-labels.idx1-ubyte");
    dh->split_data();
    dh->count_classes();
}
