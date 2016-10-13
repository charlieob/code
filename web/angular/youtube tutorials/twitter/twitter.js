angular.module('Twitter', ['ngResource']);

function TwitterCtrl($scope, $resource) {
	$scope.twitter = $resource('http://search.twitter.com/:action',
	{action:'search.json', q:'angularjs'},
	{get:{method:'JSONP'}});
	$scope.twitter.get();
}