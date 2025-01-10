#include <stdio.h>
#include <string.h>
#include <string>
#include "GlobalJumpAligner.hpp"

using namespace std;

string to_string(ALIGNPATH::path_t& path) {
	string s;
	for ( auto&& segment : path ) {
		s.append(to_string(segment.length));
		s += segment_type_to_cigar_code(segment.type);
	}
	return s;
}

int main(int argc, char* argv[]) {
	
	// check args
	if ( argc != 7 ) {
		fprintf(stderr, "wrong score arguments:\n");
		fprintf(stderr, "usage: %s <match> <mismatch> <open> <extend> <offedge> <jump>\n", argv[0]);
		exit(-1);
	}

	// configure aligner
	AlignmentScores<int> scores(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), false);
	GlobalJumpAligner<int> aligner(scores, atoi(argv[6]));

	// loop on reading lines and process
	static char line[100000];
	int lineno = 0;
	printf("score\tjumpInsertSize\tjumpRange\tbeginPos1\tapath1\tbeginPos2\tapath2\n");
	while ( fgets(line, sizeof(line), stdin) ) {
		lineno++;
		if (!line[0] || line[0] == '#' ) {
			continue;
		}
		char* seq = strtok(line, "\t");
		char* ref1 = seq ? strtok(nullptr, "\t") : nullptr;
		char* ref2 = ref1 ? strtok(nullptr, "\t") : nullptr;
		if ( !seq || !ref1 || !ref2 ) {
			fprintf(stderr, "failed to read or parse input line #%d\n", lineno);
		}

		JumpAlignmentResult<int> result;
		jumpAlign<GlobalJumpAligner<int>,const char*,int>(
				aligner, seq, seq + strlen(seq), ref1, ref1 + strlen(ref1), ref2, ref2 + strlen(ref2), result);

		
		string apath1 = to_string(result.align1.apath);
		string apath2 = to_string(result.align2.apath);
		printf("%d\t%d\t%d\t%d\t%s\t%d\t%s\n", 
					result.score, result.jumpInsertSize, result.jumpRange,
					result.align1.beginPos, apath1.c_str(),
					result.align2.beginPos, apath2.c_str()
					);
	}

	return 0;
	
}
