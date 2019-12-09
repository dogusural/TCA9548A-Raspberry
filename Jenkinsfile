node('builder'){
	try {
		docker.image('riddleandcode/wallet-builder:latest-pi2').inside{
			stage('Building Binaries') {
				scmv = checkout scm
                sh 'cmake --version'
                stash(name: 'RunScript', includes: 'run.sh')

                dir ("SEAL")
                {
                    git url : 'git@github.com:RiddleAndCode/S.E.A.L..git'

                    dir("build_v1")
                    {
                        sh 'pwd'
                        sh 'cmake ../ -DTARGET_GROUP=v1 -DDYNAMIC_LINKING=OFF -DCMAKE_TOOLCHAIN_FILE=/xcompiler4pi/toolchain-rpi.cmake'
                        sh 'make -j$(nproc)'
                    }

                    dir("build_v2")
                    {
                        sh 'pwd'
                        sh 'cmake ../ -DTARGET_GROUP=v2 -DDYNAMIC_LINKING=OFF -DCMAKE_TOOLCHAIN_FILE=/xcompiler4pi/toolchain-rpi.cmake'
                        sh 'make -j$(nproc)'
                    }

                    dir("build_v3")
                    {
                        sh 'pwd'
                        sh 'cmake ../ -DTARGET_GROUP=v3 -DDYNAMIC_LINKING=OFF -DCMAKE_TOOLCHAIN_FILE=/xcompiler4pi/toolchain-rpi.cmake'
                        sh 'make -j$(nproc)'
                    }
                    stash(name: 'BinaryTests', includes: 'build_v1/test/v1_tests , build_v2/test/v2_tests , build_v3/test/v3_tests , build_v1/test/ses_tests')

                }

			}
		}
		node('dodo') {
			stage('Running Binaries') {
				unstash 'BinaryTests'
				unstash 'RunScript'


				sh run.sh
			}
		}
	}
	catch (error) {
		echo "The pipeline failed"
		throw error
	}
	finally {
		sh "git clean -xfd"
	}
}
