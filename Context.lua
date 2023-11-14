local classes = require("classes")

local Context = classes.class()

function Context:init(params)
    print("Context:init")
    
    self.name = params.name
    self.width = params.width
    self.height = params.height
end

return Context