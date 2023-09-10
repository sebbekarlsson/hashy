f = open('MOCK_DATA.txt')
lines = f.readlines()



template = '''
#ifndef BENCH_MOCK_DATA_H
#define BENCH_MOCK_DATA_H
#define BENCH_MOCK_DATA_LENGTH $LENGTH
typedef struct {
  const char* key;
  const char* value;
} BenchMockSample;

static const BenchMockSample samples[] = {
  $SAMPLES
};
#endif
'''

sample_template = '''
(BenchMockSample){
  .key = $KEY,
  .value = $VALUE
}
'''

def make_sample(key, value):
    return sample_template.replace('$KEY', key).replace('$VALUE', value)


samples = []

for i, line in enumerate(lines):
    if not ',' in line:
        continue
    
    kv = line.split(',')
    key, value = kv[0], kv[1]

    sample = make_sample(key, value) 

    samples.append(sample)


content = template.replace('$LENGTH', str(len(samples))).replace('$SAMPLES', ','.join(samples))

print(content)
