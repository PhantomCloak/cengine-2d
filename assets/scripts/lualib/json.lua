local json = {}
do
 
  local valuePipes = {}
  
  local function pipeValue(json, copyPos, pos)
    local cPos, c = json:match('()(%S)', pos)
    if not c then
      error('unexpected end of input')
    end
    local pipe = valuePipes[c]
    if not pipe then
      error('invalid JSON at: ' .. cPos)
    end
    return pipe(json, copyPos, cPos)
  end
 
  function json.parse(json)
    local chunk = assert(load(coroutine.wrap(function()
      coroutine.yield('return ')
      local copyPos, pos = pipeValue(json, 1, 1)
      local invalidPos = json:match('()%S', pos)
      if invalidPos then
        error('invalid JSON at: ' .. invalidPos)
      end
      if (copyPos < pos) then
        coroutine.yield(json:sub(copyPos))
      end
    end)))
    return chunk()
  end
  
  local function pipeStringContent(json, copyPos, pos)
    while true do
      local cPos, c = json:match('()(["\\])', pos)
      if (c == '"') then
        return copyPos, cPos+1
      elseif (c == '\\') then
        local escape = json:sub(cPos+1, cPos+1)
        if (escape == 'u') then
          local code = tonumber(json:sub(cPos+2, cPos+5), 16)
          if not code then
            error('invalid string escape')
          end
          if (code < 0x80) then
            code = string.format('\\%03d', code)
          elseif (code < 0x800) then
            code = string.format('\\%03d\\%03d',
              0xC0 + math.floor(code/0x40),
              0x80 + code%0x40)
          else
            code = string.format('\\%03d\\%03d\\%03d',
              0xE0 + math.floor(code/0x1000),
              0x80 + math.floor(code/0x40)%0x40,
              0x80 + code%0x40)
          end
          coroutine.yield(json:sub(copyPos, cPos-1) .. code)
          copyPos = cPos+6
          pos = copyPos
        elseif escape == '/' then
          coroutine.yield(json:sub(copyPos, cPos-1) .. '/')
          pos = cPos+2
          copyPos = pos
        elseif escape:match('["\\bfnrt]') then
          pos = cPos+2
        else
          error('invalid string escape')
        end
      else
        error('unexpected end of input')
      end
    end
  end
  
  valuePipes['['] = function(json, copyPos, pos)
    coroutine.yield(json:sub(copyPos, pos-1) .. 'json.array{')
    copyPos = pos+1
    pos = copyPos
    local emptyPos = json:match('^%s*%]()', copyPos)
    if emptyPos then
      coroutine.yield('}')
      return emptyPos, emptyPos
    end
    while true do
      copyPos, pos = pipeValue(json, copyPos, pos)
      local cPos, c = json:match('()(%S)', pos)
      if not c then
        error('unexpected end of input')
      end
      if (c == ']') then
        coroutine.yield(json:sub(copyPos, cPos-1) .. '}')
        return cPos+1, cPos+1
      elseif (c ~= ',') then
        error('invalid JSON at: ' .. cPos)
      end
      pos = cPos+1
    end
  end
  valuePipes['"'] = function(json, copyPos, pos)
    return pipeStringContent(json, copyPos, pos+1)
  end
  valuePipes['{'] = function(json, copyPos, pos)
    pos = pos + 1
    local emptyPos = json:match('^%s*%}()', pos)
    if emptyPos then
      return copyPos, emptyPos
    end
    while true do
      local keyPos, key = json:match('^%s*()(%S)', pos)
      if (key ~= '"') then
        if not keyPos then
          error('unexpected end of input')
        end
        error('invalid JSON at: ' .. pos)
      end
      coroutine.yield(json:sub(copyPos, keyPos-1) .. '["')
      copyPos, pos = pipeStringContent(json, keyPos+1, keyPos+1)
      local valPos, v = json:match('^%s*()(%S)', pos)
      if (v ~= ':') then
        if not valPos then
          error('unexpected end of input')
        end
        error('invalid JSON at: ' .. pos)
      end
      coroutine.yield(json:sub(copyPos, pos-1) .. ']=')
      copyPos, pos = pipeValue(json, valPos+1, valPos+1)
      local cPos, c = json:match('()(%S)', pos)
      if not c then
        error('unexpected end of input')
      end
      pos = cPos + 1
      if (c == '}') then
        return copyPos, pos
      elseif (c ~= ',') then
        error('invalid JSON at: ' .. cPos)
      end
    end
  end
  valuePipes['t'] = function(json, copyPos, pos)
    assert(json:sub(pos, pos+3) == 'true', 'invalid JSON at: ' .. pos)
    return copyPos, pos+4
  end
  valuePipes['f'] = function(json, copyPos, pos)
    assert(json:sub(pos, pos+4) == 'false', 'invalid JSON at: ' .. pos)
    return copyPos, pos+5
  end
  valuePipes['n'] = function(json, copyPos, pos)
    assert(json:sub(pos, pos+3) == 'null', 'invalid JSON at: ' .. pos)
    coroutine.yield(json:sub(copyPos, pos-1) .. 'json.')
    return pos, pos+4
  end
  local function numParser(json, copyPos, pos)
    local afterPos = json:match('^%-?[0-9]+()', pos)
    if not afterPos then
      error('invalid JSON at: ' .. pos)
    end
    afterPos = json:match('^%.[0-9]+()', afterPos) or afterPos
    afterPos = json:match('^[eE][%+%-]?[0-9]+()', afterPos) or afterPos
    return copyPos, afterPos
  end
  for i = 0, 9 do
    valuePipes[tostring(i)] = numParser
  end
  valuePipes['-'] = numParser
  
  -- override to replace default functionality
  json.null = nil
  json.array = function(a)
    return a
  end
  
end
return json
 