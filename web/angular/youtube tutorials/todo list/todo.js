function TodoCtrl($scope) {
	$scope.todos = [
		{text:'Learn AngularJS', done:false},
		{text:'Build an App', done:false}
	]
	
	$scope.getTotalTodos = function() {
		return $scope.todos.length;
	};
	
	$scope.remaining = function() {
    var count = 0;
    angular.forEach($scope.todos, function(todo) {
      count += todo.done ? 0 : 1;
    });
    return count;
  };
	
	$scope.clearCompleted = function() {
 		var oldTodos = $scope.todos;
    $scope.todos = [];
    angular.forEach(oldTodos, function(todo) {
      if (!todo.done) $scope.todos.push(todo);
		});
};

	$scope.addTodo = function() {
		$scope.todos.push({text:$scope.formTodoText, done:false});
		$scope.formTodoText = '';
	};
}