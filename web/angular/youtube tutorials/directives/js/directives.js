angular.module('components', [])
	.directive('helloWorld', function() {
		return {
			restrict: 'E',
			scope:{
				name: '@'   /* was 'bind' in the tutorial, syntax apparently changed since */
			},
			templateUrl: 'partials/hello.html'
		}
	})
	
angular.module('HelloApp', ['components'])